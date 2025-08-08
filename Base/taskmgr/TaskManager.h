//
// Created by secon on 2025/3/11.
//

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

class TaskManager
{
public:
    TaskManager();

    void addTask(const std::string &description, int priority, const std::string &dueDate);
    void deleteTask(int id);
    void updateTask(int id, const std::string &description, int priority, const std::string &dueDate);
    void listTasks(int sortOption = 0) const; // 0-按ID, 1-按优先级, 2-按截止日期
    void loadTasks();
    void saveTasks() const;

private:
    std::vector<Task> tasks;
    int nextId;

    // 辅助排序函数
    static bool compareByPriority(const Task &a, const Task &b);
    static bool compareByDueDate(const Task &a, const Task &b);
};

#endif // TASKMANAGER_H
