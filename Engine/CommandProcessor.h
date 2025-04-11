#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <algorithm>

class CommandProcessor {
public:
    using CommandFunc = std::function<std::string(const std::vector<std::string>& args)>;

    // Регистрация команды
    void registerCommand(const std::string& name, CommandFunc func) {
        std::string lowerName = toLower(name);
        commands[lowerName] = func;
        availableCommands.push_back(lowerName);
    }

    // Выполнение команды по строке
    std::string executeCommand(const std::string& commandLine) {
        std::vector<std::string> tokens = tokenize(commandLine);
        if (tokens.empty()) return "void";

        std::string cmdName = toLower(tokens[0]);
        auto it = commands.find(cmdName);
        if (it != commands.end()) {
            return "Function returned: " + it->second(tokens);
        }
        else {
            return "Unknown command: " + tokens[0];
        }
    }

    // Получает список вариантов автодополнения для введённого частичного текста (только для команд)
    std::vector<std::string> getSuggestions(const std::string& partial) {
        std::vector<std::string> suggestions;
        std::string lowerPartial = toLower(partial);
        for (const auto& cmd : availableCommands) {
            // Если команда начинается с введённого фрагмента
            if (cmd.find(lowerPartial) == 0) {
                suggestions.push_back(cmd);
            }
        }
        return suggestions;
    }
private:
    std::unordered_map<std::string, CommandFunc> commands;
    // Хранение доступных команд (в нижнем регистре)
    std::vector<std::string> availableCommands;

    // Преобразование в нижний регистр
    std::string toLower(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    // Токенизация строки по пробелам
    std::vector<std::string> tokenize(const std::string& str) {
        std::istringstream iss(str);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

extern CommandProcessor cmdBuffer;
