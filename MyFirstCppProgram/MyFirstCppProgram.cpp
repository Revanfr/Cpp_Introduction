#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <fstream> // For saving
#include <iomanip> // For loading

int playerMoney = 50;
int basePlayerMoney = 50;
int currentDay;

// here I decided that each items would contains a name, a unique id, a cost and a damage value.
class itemsData
{
public:
    std::string name;
    int id;
    int cost;
    int durability;
};

std::vector<itemsData> itemsInBackpack;
std::vector<itemsData> itemsId;

// here I setted up function 'prototypes' so that I can freely call them without being bothered by there places in the script.
// | global prototype.
void commandSheet();
void feedCommand(std::string commandToExecute);
void saveData();
void loadData();
void gameOver();
void resetGame();
void codex();
// | shop prototype.
void openShop();
void buy(int itemIdToPurchase, itemsData itemPrice);
void addToBackpack(int itemToAddId);
// | backpack prototype.
void displayBackpack();
// | sell prototype.
void sellShop();
void sellItem(int itemToSellId);
// | expedition prototype.
void expeditionSettings();
void expedition(float successProba);
void expeditionConfirmation(float successProba);
void expeditionOutcome(bool isSuccess);

/* FOR SAVING: 1 = load | 2 = save | 0 or other = normal*/

// here I create my items profiles and add them to the shop array + call commandSheet().
int initialize()
{
    itemsId.clear();

    itemsData item;
    item.name = "sword";
    item.id = 1;
    item.cost = 10;
    item.durability = 5;
    itemsId.push_back(item);

    itemsData item2;
    item2.name = "shield";
    item2.id = 2;
    item2.cost = 5;
    item2.durability = 10;
    itemsId.push_back(item2);

    commandSheet();

    return 0;
}

// here I call the initialize() function (this also allows me to update the items prices as I wish). Everytime that the player reaches the end of a cycle he is directed here.
int main(int loadStatus)
{
    if (loadStatus == 1)
    {
        loadData();
        initialize();
    }
    else if (loadStatus == 2)
    {
        saveData();
        initialize();
    }
    else 
    {
        initialize();
    }

    return 0;
}

void saveData()
{
    std::ofstream saveFile("save.txt");

    saveFile << playerMoney << std::endl;
    saveFile << currentDay << std::endl;
    for (itemsData item : itemsInBackpack)
    {
        saveFile << std::quoted(item.name) << " " << item.id << " " << item.cost << " " << item.durability << std::endl;
     }

    saveFile.close();
}

void loadData()
{
    std::ifstream saveFile("save.txt");

    if (saveFile.is_open())
    {
        saveFile >> playerMoney;
        saveFile >> currentDay;
        
        itemsInBackpack.clear();
        itemsData item;
        while(saveFile >> std::quoted(item.name) >> item.id >> item.cost >> item.durability)
        {
            itemsInBackpack.push_back(item);
        }

        saveFile.close();
    }
}

// here I simply display the commands accepted to interact with the script. I also call feedCommand() to start the following of the logic.
void commandSheet()
{
    std::string command;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "The current day is day " << currentDay << std::endl;
    std::cout << "Here are the list of command:" << std::endl;
    std::cout << "shop | backpack | sell | expedition | codex | reset" << std::endl;

    std::cout << "I would like to execute command: ";
    std::cin >> command;
    feedCommand(command);
}

// here I feed the script a command provided by the user and redirect him toward the correct function (buy - backpack - sell...)
void feedCommand(std::string commandToExecute)
{
    std::string wannaResetGame;
    if (commandToExecute == "shop")
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        openShop();
    }
    else if (commandToExecute == "backpack")
    {
        displayBackpack();
    }
    else if (commandToExecute == "sell")
    {
        sellShop();
    }
    else if (commandToExecute == "expedition")
    {
        expeditionSettings();
    }
    else if (commandToExecute == "codex")
    {
        codex();
    }
    else if (commandToExecute == "reset")
    {
        std::cout << std::endl;
        std::cout << "Are you sure you want to reset your game? All progress will be lost.  Yes/No: ";
        std::cin >> wannaResetGame;

        if (wannaResetGame == "Yes" || wannaResetGame == "yes")
        {
            resetGame();
        }
        else if (wannaResetGame == "No" || wannaResetGame == "no")
        {
            std::cout << std::endl;
            std::cout << "Aborting reset" << std::endl;
            std::cout << std::endl;
            main(0);
        }
    }
    else
    {
        // In the event the command is unknown
        std::cout << "- Unknown command, please try again -" << std::endl;
        std::cout << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << std::endl;
        main(0);
    }
}

// Here I handle the Game Over display. I also call resetGame() to reset the player's inventory;
void gameOver()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "GAME OVER" << std::endl;
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "You've run out of Euros. Without money to secure supplies, equipment, or basic necessities, your chances of survival are now next to none." << std::endl;
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;
    resetGame();
}

// Here I handle the reset logic. I also call main() to restart the cycle.
void resetGame()
{
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Resetting Your Game:" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    currentDay = 0;
    std::cout << "Your day progression has been resetted" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    playerMoney = basePlayerMoney;
    std::cout << "Your saving has been resetted" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    itemsInBackpack.clear();
    std::cout << "Your backpack has been resetted" << std::endl;
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;
    main(2);
}

// Here I handle the display of the codex (lore) to the player. I also call main() to restart the cycle.
void codex()
{
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Nobody knows exactly when the world ended. Some blame the wars, others the disease, the collapse of governments, or the years of disasters that followed. Whatever the cause, Europe as we knew it is gone." << std::endl;
    std::cout << "Cities have fallen silent. Highways have become graveyards of abandoned vehicles. Countries have lost their borders, governments their authority, and entire populations have disappeared. What remains is a vast wasteland scattered with the ruins of the old world." << std::endl;
    std::cout << std::endl;
    std::cout << "Those who survived have learned to live differently. Small groups of people have gathered in makeshift camps, hiding between abandoned buildings, forests, and forgotten settlements. These camps are all that remain of civilization. They offer shelter, protection, and sometimes a chance to start over. But nowhere is truly safe, and resources are never guaranteed." << std::endl;
    std::cout << "Yet, amidst the collapse, one thing has survived: the Euro." << std::endl;
    std::cout << std::endl;
    std::cout << "No governments remain to issue it, no banks to protect it, and no economy to give it meaning. Still, its value has endured. Across the wastelands, survivors have agreed on one universal currency. Whether you're trading with a scavenger in old-France, a merchant in old-Germany, or a camp leader somewhere in the ruins of old-Italy, Euros are the only accepted form of payment." << std::endl;
    std::cout << std::endl;
    std::cout << "Nothing comes free anymore. Food, clean water, medicine, ammunition, shelter, even a place around a campfire, everything has a price. Trust is rare, generosity is rarer, and nobody can afford to give away what might keep them alive tomorrow." << std::endl;
    std::cout << "For some, Euros are simply a means of trade. For others, they are the difference between another day of life and an early grave." << std::endl;
    std::cout << "You are one of those survivors." << std::endl;
    std::cout << std::endl;
    std::cout << "Out there, every expedition is a gamble. You leave the safety of your camp in search of supplies, valuables, and anything worth bringing back. Sometimes you return richer. Sometimes you return empty-handed. And sometimes, you don't return at all." << std::endl;
    std::cout << "In this world, survival isn't about how strong you are, how far you can travel, or how many enemies you can kill." << std::endl;
    std::cout << "It's about how many Euros you have left." << std::endl;
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Welcome to the wasteland of Europe." << std::endl;
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;
    main(0);
}

// || SHOP ||
// here is where I display the store catalogue to the player and ask him what he wants to buy.
void openShop()
{
    int itemIdToPurchase;

    std::cout << std::endl;
    std::cout << "Here is the list of items that can be bought and added to your backpack: " << std::endl;

    for (itemsData currentItem : itemsId)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << currentItem.id << " - " << currentItem.name << " | The cost for this item is " << currentItem.cost << " euros. | It has a durability of " << currentItem.durability << "." << std::endl;

        if (currentItem.id == itemsId.size())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "You currently have " << playerMoney << " euros left." << std::endl;
            std::cout << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "Which item ID would you like to purchase? " << std::endl;
            std::cout << "I whish to purchase item ID ";
            std::cin >> itemIdToPurchase;

            if (itemIdToPurchase != 0 && itemIdToPurchase <= itemsId.size())
            {
                buy(itemIdToPurchase, itemsId[itemIdToPurchase - 1]);
            }
            else
            {
                //std::cout << itemIdToPurchase << " - " << itemsId.size(); ONLY FOR DEBUGGING
                std::cout << "The item ID you entered doesn't exist, please try again." << std::endl;
                std::cout << std::endl;
                std::cout << "--------------------------------------" << std::endl;
                std::cout << std::endl;
                main(0);
            } 
        }
    }
}
// here I handle the money spending logic and call to addToBackpack().
void buy(int itemIdToPurchase, itemsData itemPrice)
{
    if (playerMoney >= itemPrice.cost)
    {
        playerMoney = playerMoney - itemPrice.cost;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "You succesfully purchased: " << itemPrice.name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "You have " << playerMoney << " euros left." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        addToBackpack(itemIdToPurchase); 
    }
    else if (playerMoney < itemPrice.cost)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "You have not enough money to buy this item!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        main(0);

    }
}
// here I add the bought item to the player backpack. I also call main() to restart the cycle.
void addToBackpack(int itemToAddId)
{
    itemsInBackpack.push_back(itemsId[itemToAddId - 1]);
    std::cout << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;
    main(2);
}


// || BACKPACK ||
// here I simply display what is in the backpack. I also call main() to restart the cycle.
void displayBackpack()
{
    std::cout << std::endl;
    std::cout << "In your backpack you have :" << std::endl;

    if (itemsInBackpack.size() == 0)
    {
        std::cout << "Your backpack is empty!" << std::endl;
        std::cout << "You currently have " << playerMoney << " euros that can be used in the shop." << std::endl;
        std::cout << "The current day is day " << currentDay << std::endl;
        std::cout << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << std::endl;
        main(2);
    }
    else
    {
        for (itemsData currentItemDisplayed : itemsInBackpack)
        {
            std::cout << currentItemDisplayed.id << " - " << currentItemDisplayed.name << " which cost " << currentItemDisplayed.cost << " euros." << std::endl;
        }
        std::cout << "You currently have " << playerMoney << "  euros that can be used in the shop." << std::endl;
        std::cout << "The current day is day " << currentDay << std::endl;
        std::cout << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << std::endl;
        main(2);
    }  
}


// || SELL ||
// here I find the location in the backpack array of the item that the user wants to sell using the items' unique id.
void sellShop()
{
    int itemIdToSell;
    std::cout << std::endl;

    if (itemsInBackpack.size() == 0)
    {
        std::cout << "Your backpack is empty, please buy something in the shop before." << std::endl;
        std::cout << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << std::endl;
        main(0);
    }
    else 
    {
        std::cout << "Which item from your backpack you wish to sell:" << std::endl;
        for (itemsData currentItemDisplayed : itemsInBackpack)
        {
            std::cout << currentItemDisplayed.id << " - " << currentItemDisplayed.name << " which cost " << currentItemDisplayed.cost << " euros." << std::endl;
        }
        std::cout << std::endl;
        std::cout << "I wish to sell item ID ";
        std::cin >> itemIdToSell;

        auto it = std::find_if(itemsInBackpack.begin(), itemsInBackpack.end(),
            [itemIdToSell](const itemsData& item)
            {
                return item.id == itemIdToSell;
            });

        if (it != itemsInBackpack.end())
        {
            int position = std::distance(itemsInBackpack.begin(), it);

            sellItem(position);
        }
        else
        {
            std::cout << "The item ID you entered doesn't exist, please try again." << std::endl;
            std::cout << std::endl;
            std::cout << "--------------------------------------" << std::endl;
            std::cout << std::endl;
            main(0);
        }
    }
}
// here I handle the logic of removing the item from the backpack and refund the player. I also call main() to restart the cycle.
void sellItem(int itemToSellId)
{
    std::string itemSoldName;

    playerMoney = playerMoney + itemsInBackpack[itemToSellId].cost;
    itemSoldName = itemsInBackpack[itemToSellId].name;
    itemsInBackpack.erase(itemsInBackpack.begin() + itemToSellId);
    std::cout << "You succesfully sold: " << itemSoldName << std::endl;
    std::cout << "Your new balance is now " << playerMoney << " euros." << std::endl;
    std::cout << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << std::endl;
    main(2);
}


// || EXPEDITION ||
// here I ... 
void expeditionSettings()
{
    std::string improveExpeditionOdds;
    float expeditionSuccesRate;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1);
    expeditionSuccesRate = distribution(gen);

    std::cout << std::endl;
    std::cout << "Here are your odds for this expedition success: " << expeditionSuccesRate * 100 << "%" << std::endl;
    // MAYBE LATER IMPROVE SYSTEM HERE TO EVADE SPAMMING FOR BETTER SUCCESS RATE!!!
    expeditionConfirmation(expeditionSuccesRate);
    /*
    std::cout << "Are you willing to improve your odds for this expedition?  Yes/No: ";
    std::cin >> improveExpeditionOdds;
    if (improveExpeditionOdds == "Yes" || improveExpeditionOdds == "yes")
    {
        // TRADE SOMETHING FOR BETTER ODDS!!!
        expeditionConfirmation(expeditionSuccesRate);
    }
    else if (improveExpeditionOdds == "No" || improveExpeditionOdds == "no")
    {
        expeditionConfirmation(expeditionSuccesRate);
    }
    */
}
// here I ... 
void expeditionConfirmation(float successProba)
{
    std::string startExpedition;
    std::cout << "Do you want to start an expedition?  Yes/No: ";
    std::cin >> startExpedition;
    if (startExpedition == "Yes" || startExpedition == "yes")
    {
        expedition(successProba);
    }
    else if (startExpedition == "No" || startExpedition == "no")
    {
        std::cout << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << std::endl;
        main(2);
    }
}
// here I ... 
void expedition(float successProba)
{
    std::cout << std::endl;
    std::cout << "- The expedition has started -" << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution distribution(successProba);

    bool isSuccessful = distribution(gen);
    expeditionOutcome(isSuccessful);
}
// here I ... 
void expeditionOutcome(bool isSuccess)
{
    int todayExpense;

    if (isSuccess)
    {
        std::cout << "Your expedition was a success!" << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(1, 10);
        int randomNumber = distribution(gen);
        std::cout << "You earned +" << randomNumber << " euros from the expedition!" << std::endl;
        playerMoney = playerMoney + randomNumber;

        std::random_device rd1;
        std::mt19937 gen1(rd());
        std::uniform_int_distribution<int> distribution1(1, 5);
        int todayExpense = distribution(gen);
        currentDay++;
        playerMoney = playerMoney - todayExpense;
        std::cout << "Today's expenses: -" << todayExpense << " euros." << std::endl;
        std::cout << std::endl;
        std::cout << "You currently have " << playerMoney << " euros that can be used in the shop." << std::endl;

        std::cout << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << std::endl;

        if (playerMoney <= 0)
        {
            gameOver();
        }
        else
        {
            main(2);
        }
    }
    else
    {
        std::cout << "Your expedition failed!" << std::endl;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(1, 5);
        int randomNumber = distribution(gen);
        std::cout << "You couldn't recover your investment. You lost  -" << randomNumber << " euros from the expedition!" << std::endl;
        playerMoney = playerMoney - randomNumber;

        std::random_device rd1;
        std::mt19937 gen1(rd());
        std::uniform_int_distribution<int> distribution1(1, 5);
        int todayExpense = distribution(gen);
        currentDay++;
        playerMoney = playerMoney - todayExpense;
        std::cout << "Today's expenses: -" << todayExpense << " euros." << std::endl;
        std::cout << std::endl;
        std::cout << "You currently have " << playerMoney << " euros that can be used in the shop." << std::endl;

        if (playerMoney <= 0)
        {
            gameOver();
        }
        else
        {
            std::cout << std::endl;
            std::cout << "--------------------------------------" << std::endl;
            std::cout << std::endl;
            main(2);
        }   
    }
}
