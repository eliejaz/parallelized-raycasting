#include <chrono>
#include <iostream>
#include <map>
#include <memory>

#include <Player.h>
#include <Map.h>
#include <WindowManager.h>
#include <Raycaster.h>
#include <UDPReceiver.h>
#include <UDPSender.h>
#include <util.h>
#include <omp.h>
#include <thread>
#include <atomic>

struct ProgramArguments
{
    int screenWidth;
    int screenHeight;
    int numThreads;
    std::string ipsPath;
};

ProgramArguments parseArgs(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <screenWidth> <screenHeight> <ipsPath> <numThreads>" << std::endl;
        std::cerr << "  screenWidth: The width of the screen." << std::endl;
        std::cerr << "  screenHeight: The height of the screen." << std::endl;
        std::cerr << "  ipsPath: The path to the file containing the IP addresses and ports of the players." << std::endl;
        std::cerr << "Example: " << argv[0] << " 1920 1080 ips.txt " << std::endl;
        exit(1);
    }

    ProgramArguments args;
    args.screenWidth = std::stoi(argv[1]);
    args.screenHeight = std::stoi(argv[2]);
    args.ipsPath =  argv[3];
    args.numThreads = 1;
    return args;
}

void receivePlayersPositionsInParallelThread(UDPReceiver* udpReceiver,
                                             size_t nbPlayers,
                                             std::map<std::string, int>* playerIndexes,
                                             Map* map,
                                             std::atomic<bool>* isRunning) {
    int playerIndex = 0;
    while (isRunning->load()) {
        for (size_t i = 0; i < nbPlayers; i++) {
            try {
                auto data = udpReceiver->receive();
                if (playerIndexes->find(data.first) == playerIndexes->end()) {
                    (*playerIndexes)[data.first] = playerIndex++;
                    playerIndex %= nbPlayers;
                }
                int index = (*playerIndexes)[data.first];
                map->movePlayer(index, data.second.x(), data.second.y());
            } catch (...) {
            }
        }
    }
}

void sendPlayerPositionInParallelThread(const std::vector<std::unique_ptr<UDPSender>>* udpSenders,
                                        const Player* player,
                                        std::atomic<bool>* isRunning) {
    while (isRunning->load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto posX = player->posX();
        auto posY = player->posY();
        for (const auto& udpSender : *udpSenders) {
            udpSender->send(posX, posY);
        }
    }
}

int main(int argc, char *argv[])
{
    ProgramArguments args = parseArgs(argc, argv);
    omp_set_num_threads(args.numThreads);
    const int screenWidth = args.screenWidth;
    const int screenHeight = args.screenHeight;

    std::vector<std::unique_ptr<UDPSender>> udpSenders;
    NetworkData data = parseIPs(args.ipsPath);
    UDPReceiver udpReceiver(data.listeningPort);
    for (auto ipPort : data.ipPorts)
        udpSenders.push_back(std::unique_ptr<UDPSender>(new UDPSender(ipPort.first, ipPort.second)));
    size_t nbPlayers = udpSenders.size();
    std::map<std::string, int> playerIndexes;

    Map map = Map::generateMap(nbPlayers);
    Player player({22, 11.5}, {-1, 0}, {0, 0.66}, 5, 3, map);
    WindowManager windowManager(screenWidth, screenHeight);
    InputManager &inputManager = windowManager.getInputManager();
    Raycaster raycaster(player, windowManager, map);

    std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now(), oldTime;

    Texture floorTexture = map.getFloorTexture();
    Texture ceilingTexture = map.getCeilingTexture();

    std::atomic<bool> isRunning(true);
    std::thread playerRecieveThread(receivePlayersPositionsInParallelThread,
                              &udpReceiver,
                              nbPlayers,
                              &playerIndexes,
                              &map,
                              &isRunning);
    std::thread playerSendThread(sendPlayerPositionInParallelThread,
                                &udpSenders,
                                &player,
                                &isRunning);

    while (true)
    {
        raycaster.castFloorCeiling();
        raycaster.castWalls();
        raycaster.castSprites();

        oldTime = time;
        time = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = time - oldTime;
        double frameTime = elapsed.count();

        windowManager.updateFPS(1.0 / frameTime);
        windowManager.flush();

        inputManager.update();

        if (inputManager.up())
            player.move(frameTime);
        if (inputManager.down())
            player.move(-frameTime);
        if (inputManager.right())
            player.turn(-frameTime);
        if (inputManager.left())
            player.turn(frameTime);
        if (inputManager.esc())
            break;

    }
    isRunning = false;
    playerRecieveThread.join();
    playerSendThread.join();

}