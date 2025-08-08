#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "TaskManager.h"
#include "Command.h"
#include <functional>
#include <variant>

int main()
{
    system("chcp 65001 > nul");
    TaskManager taskManager;

    // 方式一、基类抽象虚函数
    //  创建命令对象
    //    AddCommand addCmd(taskManager);
    //    DeleteCommand delCmd(taskManager);
    //    ListCommand listCmd(taskManager);
    //    UpdateCommand updateCmd(taskManager);
    //
    //    // 命令映射
    //    std::unordered_map<std::string, std::unique_ptr<CommandBase>> commands;
    //    commands["add"] = std::make_unique<AddCommand>(taskManager);
    //    commands["delete"] = std::make_unique<DeleteCommand>(taskManager);
    //    commands["list"] = std::make_unique<ListCommand>(taskManager);
    //    commands["update"] = std::make_unique<UpdateCommand>(taskManager);

    // 方式二、 类型擦除
    //      auto add_command = std::make_shared<AddCommand>(taskManager);
    //      auto delete_command = std::make_shared<DeleteCommand>(taskManager);
    //      auto list_command = std::make_shared<ListCommand>(taskManager);
    //      auto update_command = std::make_shared<UpdateCommand>(taskManager);
    //      std::unordered_map<std::string, std::function<void(const std::string&)> > commands;
    //      commands["add"] = [add_command](const std::string& args)
    //              { add_command->execute(args); };
    //      commands["delete"] = [delete_command](const std::string& args)
    //              { delete_command->execute(args); };
    //      commands["list"] = [list_command](const std::string& args)
    //              { list_command->execute(args); };
    //      commands["update"] = [update_command](const std::string& args)
    //              { update_command->execute(args); };

    // 方式三、 std::variant
    //    using CommandVariant = std::variant<
    //            std::unique_ptr<AddCommand>,
    //            std::unique_ptr<DeleteCommand>,
    //            std::unique_ptr<ListCommand>,
    //            std::unique_ptr<UpdateCommand>
    //    >;
    //    std::unordered_map<std::string, CommandVariant> commands;
    //    commands["add"] = std::make_unique<AddCommand>(taskManager);
    //    commands["delete"] = std::make_unique<AddCommand>(taskManager);
    //    commands["list"] = std::make_unique<AddCommand>(taskManager);
    //    commands["update"] = std::make_unique<AddCommand>(taskManager);

    // 方式四 采用wrapper封装
    std::unordered_map<std::string, CommandWrapper> commands;
    commands.emplace("add", AddCommand(taskManager));
    commands.emplace("delete", DeleteCommand(taskManager));
    commands.emplace("list", ListCommand(taskManager));
    commands.emplace("update", UpdateCommand(taskManager));

    std::cout << "欢迎使用任务管理系统！" << std::endl;
    std::cout << "可用命令: add, delete, list, update, exit" << std::endl;

    std::string input;
    while (true)
    {
        std::cout << "\n> ";
        std::getline(std::cin, input);
        if (input.empty())
            continue;

        // 分离命令和参数
        size_t spacePos = input.find(' ');
        std::string cmd = input.substr(0, spacePos);
        std::string args;
        if (spacePos != std::string::npos)
        {
            args = input.substr(spacePos + 1);
        }

        if (cmd == "exit")
        {
            std::cout << "退出程序。" << std::endl;
            break;
        }
        std::cout << "cmd: " << cmd << std::endl;
        auto it = commands.find(cmd);
        if (it != commands.end())
        {
            // 方式一
            // it->second->execute(args);
            // 方式二
            // it->second(args);
            // 方式三
            //            std::visit([&args](auto&& cmdPtr)
            //            { cmdPtr->execute(args); }, it->second);
            // 方式四
            it->second.execute(args);
        }
        else
        {
            std::cout << "未知命令：" << cmd << std::endl;
        }
    }

    return 0;
}