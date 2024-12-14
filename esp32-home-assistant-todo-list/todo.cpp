#include "todo.h"

// Fetch tasks from Home Assistant
bool fetchTodos(std::vector<TodoItem> &todos) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String fetchURL = baseURL + "/states/" + listName;
    http.begin(fetchURL);
    http.addHeader("Authorization", "Bearer " + accessToken);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.GET(); // Send GET request to fetch tasks

    if (httpResponseCode == 200) {
      String response = http.getString();
      StaticJsonDocument<4000> doc; // Buffer size for JSON parsing
      DeserializationError error = deserializeJson(doc, response);

      if (!error) {
        JsonArray allTodos = doc["attributes"]["all_todos"].as<JsonArray>();
        for (JsonObject todo : allTodos) {
          TodoItem item;

          // Extract task details
          item.subject = todo["subject"].as<String>();
          item.completed = (todo["status"].as<String>() == "completed");

          // Extract due date
          if (todo.containsKey("due") && !todo["due"].isNull()) {
            item.dueDate = todo["due"].as<String>();
          } else {
            item.dueDate = "No due date";
          }

          // Extract description
          if (todo.containsKey("description") && !todo["description"].isNull()) {
            item.description = todo["description"].as<String>();
          } else {
            item.description = "No description";
          }

          todos.push_back(item);
        }
        http.end();
        return true;
      } else {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected");
  }
  return false;
}

// Add a new task to Home Assistant
bool addTodo(const String &subject, const String &dueDate, const String &description) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String addURL = baseURL + "/services/todo/add_item";
    http.begin(addURL);
    http.addHeader("Authorization", "Bearer " + accessToken);
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload for adding a task
    StaticJsonDocument<500> doc;
    JsonObject data = doc.to<JsonObject>();
    data["entity_id"] = listName; // Replace with your To-Do list entity ID
    data["item"] = subject;       // Task name

    if (!dueDate.isEmpty()) {
      data["due_date"] = dueDate; // Due date (optional)
    }

    if (!description.isEmpty()) {
      data["description"] = description; // Description (optional)
    }

    String payload;
    serializeJson(doc, payload);

    // Send POST request to add the task
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode == 200 || httpResponseCode == 201) {
      Serial.println("Task added successfully.");
      http.end();
      return true;
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
      Serial.println("Payload sent: ");
      Serial.println(payload);
    }

    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected");
  }
  return false;
}
// Function to remove a To-Do item
bool removeTodo(const String &item) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String removeURL = baseURL + "/services/todo/remove_item";
    http.begin(removeURL);
    http.addHeader("Authorization", "Bearer " + accessToken);
    http.addHeader("Content-Type", "application/json");

    // Create the payload for deletion
    StaticJsonDocument<300> doc;
    doc["entity_id"] = listName; // To-Do list entity ID
    doc["item"] = item;          // Name of the task to delete

    String payload;
    serializeJson(doc, payload);

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode == 200 || httpResponseCode == 201) {
      Serial.println("Task removed successfully.");
      http.end();
      return true;
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected");
  }
  return false;
}

// Function to update a To-Do item
bool updateTodo(const String &item, const String &newName, const String &status, const String &dueDate, const String &description) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String updateURL = baseURL + "/services/todo/update_item";
    http.begin(updateURL);
    http.addHeader("Authorization", "Bearer " + accessToken);
    http.addHeader("Content-Type", "application/json");

    // Create the payload for updating a task
    StaticJsonDocument<500> doc;
    JsonObject data = doc.to<JsonObject>();
    data["entity_id"] = listName; // To-Do list entity ID
    data["item"] = item;          // Current task name

    if (!newName.isEmpty()) {
      data["rename"] = newName; // New task name
    }

    if (!status.isEmpty()) {
      data["status"] = status; // Task status
    }

    if (!dueDate.isEmpty()) {
      data["due_date"] = dueDate; // Due date
    }

    if (!description.isEmpty()) {
      data["description"] = description; // Task description
    }

    String payload;
    serializeJson(doc, payload);

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode == 200 || httpResponseCode == 201) {
      Serial.println("Task updated successfully.");
      http.end();
      return true;
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected");
  }
  return false;
}
