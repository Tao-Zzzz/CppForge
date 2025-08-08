//
// Created by secon on 2025/3/11.
//

#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>
class CommandBase
{
public:
    virtual void execute(const std::string &args) = 0;
};

// CRTP 基类模板
template <typename Derived>
class Command
{
public:
    void execute(const std::string &args)
    {
        static_cast<Derived *>(this)->executeImpl(args);
    }
};

// 具体命令类示例
#include "TaskManager.h"
#include "Logger.h"
#include <iostream>

// 添加任务命令
class AddCommand : public Command<AddCommand>
{
public:
    AddCommand(TaskManager &manager) : taskManager(manager) {}
    void executeImpl(const std::string &args)
    {
        // 简单的参数解析：描述,优先级,截止日期
        size_t pos1 = args.find(',');
        size_t pos2 = args.find(',', pos1 + 1);
        if (pos1 == std::string::npos || pos2 == std::string::npos)
        {
            std::cout << "参数格式错误。请使用: add <描述>,<优先级>,<截止日期>" << std::endl;
            return;
        }
        std::string description = args.substr(0, pos1);
        int priority = std::stoi(args.substr(pos1 + 1, pos2 - pos1 - 1));
        std::string dueDate = args.substr(pos2 + 1);
        taskManager.addTask(description, priority, dueDate);
        std::cout << "任务添加成功。" << std::endl;
    }

private:
    TaskManager &taskManager;
};

// 删除任务命令
class DeleteCommand : public Command<DeleteCommand>
{
public:
    DeleteCommand(TaskManager &manager) : taskManager(manager) {}
    void executeImpl(const std::string &args)
    {
        try
        {
            size_t pos;
            int id = std::stoi(args, &pos);
            if (pos != args.length())
            {
                std::cout << "参数格式错误。请使用: delete <ID>" << std::endl;
                return;
            }
            taskManager.deleteTask(id);
            std::cout << "任务删除成功。" << std::endl;
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "参数格式错误。请使用: delete <ID>" << std::endl;
            return;
        }
        catch (const std::out_of_range &e)
        {
            std::cout << "ID超出范围。请使用有效的任务ID。" << std::endl;
            return;
        }
    }

private:
    TaskManager &taskManager;
};

// 列出任务命令
class ListCommand : public Command<ListCommand>
{
public:
    ListCommand(TaskManager &manager) : taskManager(manager) {}
    void executeImpl(const std::string &args)
    {
        int sortOption = 0;
        if (!args.empty())
        {
            sortOption = std::stoi(args);
        }
        taskManager.listTasks(sortOption);
    }

private:
    TaskManager &taskManager;
};

// 更新任务命令
class UpdateCommand : public Command<UpdateCommand>
{
public:
    UpdateCommand(TaskManager &manager) : taskManager(manager) {}
    void executeImpl(const std::string &args)
    {
        // 参数格式: ID,描述,优先级,截止日期
        size_t pos1 = args.find(',');
        size_t pos2 = args.find(',', pos1 + 1);
        size_t pos3 = args.find(',', pos2 + 1);
        if (pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos)
        {
            std::cout << "参数格式错误。请使用: update <ID>,<描述>,<优先级>,<截止日期>" << std::endl;
            return;
        }
        int id = std::stoi(args.substr(0, pos1));
        std::string description = args.substr(pos1 + 1, pos2 - pos1 - 1);
        int priority = std::stoi(args.substr(pos2 + 1, pos3 - pos2 - 1));
        std::string dueDate = args.substr(pos3 + 1);
        taskManager.updateTask(id, description, priority, dueDate);
        std::cout << "任务更新成功。" << std::endl;
    }

private:
    TaskManager &taskManager;
};

class CommandWrapper
{
    struct Concept
    {
        virtual void execute(const std::string &args) = 0;
        virtual ~Concept() = default;
    };

    template <typename T>
    struct Model : Concept
    {
        T command;
        Model(T cmd) : command(std::move(cmd)) {}
        void execute(const std::string &args) override
        {
            command.execute(args);
        }
    };

    std::unique_ptr<Concept> impl;

public:
    template <typename T>
    CommandWrapper(T cmd) : impl(std::make_unique<Model<T>>(std::move(cmd))) {}

    void execute(const std::string &args)
    {
        impl->execute(args);
    }
};

#endif // COMMAND_H
