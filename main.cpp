#include "database.h"
#include <cstdlib>
#include <iostream>
#include <tgbot/tgbot.h>

int main() {

    const char* token = std::getenv("TG_BOT_TOKEN");
    if (!token) {
        std::cerr << "Error: TG_BOT_TOKEN is not set" << std::endl;
        return 1;
    }

    Database database("bot.db");
    TgBot::Bot bot(token);

    bot.getEvents().onCommand("start", [&bot, &database](TgBot::Message::Ptr message) {

        int64_t userId = message->from->id;

        std::string username = message->from->username;

        database.addUser(userId, username);

        bot.getApi().sendMessage(message->chat->id, "Hi!");
        });


    bot.getEvents().onCommand("button", [&bot](TgBot::Message::Ptr message) {
        
        auto button = std::make_shared<TgBot::InlineKeyboardButton>();
        button->text = "Текст кнопки";
        button->callbackData = "button_data";

        std::vector<TgBot::InlineKeyboardButton::Ptr> row;
        row.push_back(button);

        auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
        keyboard->inlineKeyboard.push_back(row);

        
        bot.getApi().sendMessage(message->chat->id, "Выбери:", false, 0, keyboard);
        });

    // button callback
    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "button_data") {
            bot.getApi().sendMessage(query->message->chat->id, "Кнопка нажата!");
        }
        });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        std::cout << "User wrote: " << message->text << std::endl;
        if (StringTools::startsWith(message->text, "/")) {
            return; // skip commands
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
        });

    // onMessage "Games"
    bot.getEvents().onNonCommandMessage([&bot](TgBot::Message::Ptr message) {
        std::cout << "User wrote: " << message->text << std::endl;
        if (message->text == "Games") {

            auto keyboard = std::make_shared<TgBot::ReplyKeyboardMarkup>();

            auto buttonKybik = std::make_shared<TgBot::KeyboardButton>();
            buttonKybik->text = "Кинуть кубик";

            auto buttonFootball = std::make_shared < TgBot::KeyboardButton>();
            buttonFootball->text = "Football";

            auto buttonCasino = std::make_shared < TgBot::KeyboardButton>();
            buttonCasino->text = "Casino";

            keyboard->keyboard.push_back({ buttonKybik, buttonFootball, buttonCasino });
            keyboard->resizeKeyboard = true;

            bot.getApi().sendMessage(message->chat->id, "You open games ", false, 0, keyboard);


        }


        });

    // on message "throw cube"
    bot.getEvents().onNonCommandMessage([&bot](TgBot::Message::Ptr message) {
        std::cout << "User wrote: " << message->text << std::endl;
        if (message->text == "throw cube") {
            bot.getApi().sendDice(message->chat->id);
            


        }


        });

    // on message "Football"
    bot.getEvents().onNonCommandMessage([&bot](TgBot::Message::Ptr message) {
        std::cout << "User wrote: " << message->text << std::endl;
        if (message->text == "Football") {
            bot.getApi().sendDice(message->chat->id, false, 0, 0, "⚽️");



        }


        });

    // on message "Casino"
    bot.getEvents().onNonCommandMessage([&bot](TgBot::Message::Ptr message) {
        std::cout << "User wrote: " << message->text << std::endl;
        if (message->text == "Casino") {
            bot.getApi().sendDice(message->chat->id, false, 0, 0, "🎰");



        }


        });


    // on command "menu"
    bot.getEvents().onCommand("menu", [&bot](TgBot::Message::Ptr message) {

        

        auto keyboard = std::make_shared<TgBot::ReplyKeyboardMarkup>();

        auto button1 = std::make_shared<TgBot::KeyboardButton>();
        button1->text = "Games";

        auto button2 = std::make_shared<TgBot::KeyboardButton>();
        button2->text = "option 2";

        keyboard->keyboard.push_back({ button1, button2 });
        keyboard->resizeKeyboard = true;

        bot.getApi().sendMessage(message->chat->id, "You open main menu", false, 0, keyboard);
        });

    // on command "balance"
    bot.getEvents().onCommand("balance", [&bot, &database](TgBot::Message::Ptr message) {
        int64_t userId = message->from->id;

        std::string username = message->from->username;

        double balance = database.getBalance(userId);

        bot.getApi().sendMessage(message->chat->id, "your balance: " + std::to_string(balance));
        });

    bot.getEvents().onCommand("addtransaction", [&bot, &database](TgBot::Message::Ptr message) {
        int64_t userId = message->from->id;

        std::string text = message->text;

        std::string args = text.substr(text.find(' ') + 1);

        std::istringstream argsStream(args);
        double amount;
        if (!(argsStream >> amount)) {
            bot.getApi().sendMessage(message->chat->id, "Error: Incorrect amount. ");
            return;
        }

        std::string description;
        std::getline(argsStream, description);

        description.erase(0, description.find_first_not_of(' '));

        if (description.empty()) {
            description = "without description";
        }

        database.addTransaction(userId, amount, description);

        bot.getApi().sendMessage(message->chat->id, "Transaction added!");

        });

    bot.getEvents().onCommand("mytransaction", [&bot, &database](TgBot::Message::Ptr message) {
        int64_t userId = message->from->id;


        std::string text = database.myTransaction(userId);

        bot.getApi().sendMessage(
            message->chat->id,
            "```\n" + text + "```",
            false, 0, nullptr, "MarkdownV2"
        );
        });


    try {
        std::cout << "Bot username: " << bot.getApi().getMe()->username << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            std::cout << "Long poll started\n";
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        std::cout << "error: " << e.what() << std::endl;
    }

    database.~Database();

    return 0;
    }

