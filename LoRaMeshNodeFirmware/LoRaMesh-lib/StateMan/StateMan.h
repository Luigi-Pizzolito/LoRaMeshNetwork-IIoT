#pragma once
#include <Preferences.h>
#include <ArduinoJson.h>

#define APP_ID "node_state"

class StateMan {
    public:
        static StateMan& Instance() {
            inst = new StateMan();
            return inst;
        }
        
        void save() {
            String json;
            serializeJson(data, json);
            pref.putString("data", json);
            pref.end();
            // pref invalid from here on, singleton broken
            delete inst;
        }

        DynamicJsonDocument data(ESP.getMaxAllocHeap());

        String print() {
            String json;
            serializeJson(data, json);
            return json;
        }

    protected:
        StateMan() {
            // load previous state data
            pref.begin(APP_ID, false);
            String load_data = pref.getString("data", "{}");
            // Deserialise
            DeserializationError err = deserializeJson(data, load_data);
            // if succeeds, loaded directly into data document, else
            if (err || load_data == "{}") {
                // Load defaults
                //todo: 

            }
        }

        StateMan(const StateMan&);
        StateMan& operator=(const StateMan&);
        ~StateMan() {}

        Preferences pref;
        static StateMan* inst;
};