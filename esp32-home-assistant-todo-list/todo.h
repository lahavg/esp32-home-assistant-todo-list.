#ifndef TODO_H
#define TODO_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "config.h"
#include <vector>

// Structure to represent a To-Do task
struct TodoItem {
  String subject;     // Task name
  bool completed;     // Task status (true if completed)
  String dueDate;     // Due date of the task
  String description; // Task description
};

// Functions for managing To-Do tasks
bool fetchTodos(std::vector<TodoItem> &todos);                               // Fetch tasks from Home Assistant
bool addTodo(const String &item, const String &dueDate = "", const String &description = ""); // Add a new task
bool removeTodo(const String &item);                                         // Delete a task
bool updateTodo(const String &item, const String &newName = "", const String &status = "", const String &dueDate = "", const String &description = ""); // Update an existing task

#endif
