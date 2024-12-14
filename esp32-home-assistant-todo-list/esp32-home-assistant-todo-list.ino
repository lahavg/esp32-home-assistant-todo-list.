#include <Arduino.h>
#include "network.h" // Wi-Fi connection functions
#include "todo.h"    // To-Do list management functions

std::vector<TodoItem> todos; // Vector to store the fetched To-Do items

// Function to display the To-Do items
void displayTodos() {
  if (fetchTodos(todos)) {
    Serial.println("Fetched To-Do items successfully:");
    for (const auto &item : todos) {
      Serial.println("---------------------------");
      Serial.print("Task: ");
      Serial.println(item.subject);
      Serial.print("Completed: ");
      Serial.println(item.completed ? "Yes" : "No");
      Serial.print("Due Date: ");
      Serial.println(item.dueDate);
      Serial.print("Description: ");
      Serial.println(item.description);
    }
  } else {
    Serial.println("Failed to fetch To-Do items.");
  }
}

// Function to display the menu options
void displayMenu() {
  Serial.println("\n--- To-Do List Menu ---");
  Serial.println("1. Display To-Do Items");
  Serial.println("2. Update a To-Do Item");
  Serial.println("3. Add a New To-Do Item");
  Serial.println("4. Remove a To-Do Item");
  Serial.println("Enter your choice (1-4): ");
}

// Function to clear the Serial input buffer
void clearSerialBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

// Function to handle updating a To-Do item
void updateTodoItem() {
  Serial.println("Enter the name of the task to update:");
  while (!Serial.available());
  String taskName = Serial.readStringUntil('\n');
  taskName.trim();
  clearSerialBuffer();

  if (taskName.isEmpty()) {
    Serial.println("Task name cannot be empty.");
    return;
  }

  Serial.println("What would you like to update?");
  Serial.println("1. Task Name");
  Serial.println("2. Status (completed/pending)");
  Serial.println("3. Due Date");
  Serial.println("4. Description");
  while (!Serial.available());
  int choice = Serial.parseInt();
  clearSerialBuffer();

  String newName = "";
  String status = "";
  String dueDate = "";
  String description = "";

  switch (choice) {
    case 1:
      Serial.println("Enter the new task name:");
      while (!Serial.available());
      newName = Serial.readStringUntil('\n');
      newName.trim();
      clearSerialBuffer();
      break;
    case 2:
      Serial.println("Enter the new status (completed/pending):");
      while (!Serial.available());
      status = Serial.readStringUntil('\n');
      status.trim();
      clearSerialBuffer();
      break;
    case 3:
      Serial.println("Enter the new due date (YYYY-MM-DD):");
      while (!Serial.available());
      dueDate = Serial.readStringUntil('\n');
      dueDate.trim();
      clearSerialBuffer();
      break;
    case 4:
      Serial.println("Enter the new description:");
      while (!Serial.available());
      description = Serial.readStringUntil('\n');
      description.trim();
      clearSerialBuffer();
      break;
    default:
      Serial.println("Invalid choice.");
      return;
  }

  Serial.println("Do you want to proceed with the update? (y/n):");
  while (!Serial.available());
  char confirm = Serial.read();
  clearSerialBuffer();

  if (confirm == 'y' || confirm == 'Y') {
    if (updateTodo(taskName, newName, status, dueDate, description)) {
      Serial.println("Task updated successfully.");
    } else {
      Serial.println("Failed to update the task.");
    }
  } else {
    Serial.println("Update canceled.");
  }
}

// Function to handle adding a new To-Do item
void addNewTodoItem() {
  Serial.println("Enter the task name:");
  while (!Serial.available());
  String taskName = Serial.readStringUntil('\n');
  taskName.trim();
  clearSerialBuffer();

  if (taskName.isEmpty()) {
    Serial.println("Task name cannot be empty.");
    return;
  }

  Serial.println("Enter the due date (YYYY-MM-DD) or 0 if not applicable:");
  while (!Serial.available());
  String dueDate = Serial.readStringUntil('\n');
  dueDate.trim();
  clearSerialBuffer();
  if (dueDate == "0") dueDate = "";

  Serial.println("Enter the description or 0 if not applicable:");
  while (!Serial.available());
  String description = Serial.readStringUntil('\n');
  description.trim();
  clearSerialBuffer();
  if (description == "0") description = "";

  if (addTodo(taskName, dueDate, description)) {
    Serial.println("Task added successfully.");
  } else {
    Serial.println("Failed to add the task.");
  }
}

// Function to handle removing a To-Do item
void removeTodoItem() {
  Serial.println("Enter the name of the task to remove:");
  while (!Serial.available());
  String taskName = Serial.readStringUntil('\n');
  taskName.trim();
  clearSerialBuffer();

  if (taskName.isEmpty()) {
    Serial.println("Task name cannot be empty.");
    return;
  }

  if (removeTodo(taskName)) {
    Serial.println("Task removed successfully.");
  } else {
    Serial.println("Failed to remove the task.");
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  displayTodos(); // Display tasks initially
}

void loop() {
  displayMenu();
  while (!Serial.available());
  int choice = Serial.parseInt();
  clearSerialBuffer();

  switch (choice) {
    case 1:
      displayTodos();
      break;
    case 2:
      updateTodoItem();
      break;
    case 3:
      addNewTodoItem();
      break;
    case 4:
      removeTodoItem();
      break;
    default:
      Serial.println("Invalid choice. Please enter a number between 1 and 4.");
      break;
  }

  delay(1000);
}
