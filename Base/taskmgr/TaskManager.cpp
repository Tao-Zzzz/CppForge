
#include "TaskManager.h"
#include "Logger.h"
#include <iostream>

TaskManager::TaskManager() : nextId(1)
{
    loadTasks();
}

void TaskManager::addTask(const std::string &description, int priority, const std::string &dueDate)
{
    Task task;
    task.id = nextId++;
    task.description = description;
    task.priority = priority;
    task.dueDate = dueDate;
    tasks.push_back(task);
    Logger::getInstance().log("添加任务: " + task.toString());
    saveTasks();
}

void TaskManager::deleteTask(int id)
{
    auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task &task)
                           { return task.id == id; });
    if (it != tasks.end())
    {
        Logger::getInstance().log("删除任务: " + it->toString());
        tasks.erase(it);
        saveTasks();
    }
    else
    {
        std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
    }
}

void TaskManager::updateTask(int id, const std::string &description, int priority, const std::string &dueDate)
{
    for (auto &task : tasks)
    {
        if (task.id == id)
        {
            Logger::getInstance().log("更新前任务: " + task.toString());
            task.description = description;
            task.priority = priority;
            task.dueDate = dueDate;
            Logger::getInstance().log("更新后任务: " + task.toString());
            saveTasks();
            return;
        }
    }
    std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
}

void TaskManager::listTasks(int sortOption) const
{
    std::vector<Task> sortedTasks = tasks;
    switch (sortOption)
    {
    case 1:
        std::sort(sortedTasks.begin(), sortedTasks.end(), compareByPriority);
        break;
    case 2:
        std::sort(sortedTasks.begin(), sortedTasks.end(), compareByDueDate);
        break;
    default:
        break;
    }
    for (const auto &task : sortedTasks)
    {
        std::cout << task.toString() << std::endl;
    }
}

void TaskManager::loadTasks()
{
    std::ifstream inFile("tasks.txt");
    if (!inFile.is_open())
    {
        Logger::getInstance().log("任务文件不存在，开始新建。");
        return;
    }
    std::string line;
    while (std::getline(inFile, line))
    {
        std::istringstream iss(line);
        Task task;
        char delimiter;
        iss >> task.id >> delimiter;
        std::getline(iss, task.description, ',');
        iss >> task.priority >> delimiter;
        iss >> task.dueDate;
        tasks.push_back(task);
        if (task.id >= nextId)
        {
            nextId = task.id + 1;
        }
    }
    inFile.close();
    Logger::getInstance().log("加载任务成功。");
}

void TaskManager::saveTasks() const
{
    std::ofstream outFile("tasks.txt");
    if (!outFile.is_open())
    {
        Logger::getInstance().log("无法打开任务文件进行保存。");
        return;
    }
    for (const auto &task : tasks)
    {
        outFile << task.id << "," << task.description << "," << task.priority << "," << task.dueDate << "\n";
    }
    outFile.close();
    Logger::getInstance().log("保存任务成功。");
}

bool TaskManager::compareByPriority(const Task &a, const Task &b)
{
    return a.priority < b.priority;
}

bool TaskManager::compareByDueDate(const Task &a, const Task &b)
{
    return a.dueDate < b.dueDate;
}
