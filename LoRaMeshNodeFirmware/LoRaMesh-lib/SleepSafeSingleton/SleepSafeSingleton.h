#pragma once

/*
Singleton Template Class
with added private functions for sleep
persistant JSON Object store.

Make sure to store all class variables
on the *heap*, and then delete them.
*/

#include <Preferences.h>
#include <ArduinoJson.h>

template<typename T>
class SleepSafeSingleton {
    public:
        // *** Singleton ***
        static T& getInstance() {
            static T instance;
            return instance;
        }
        static void teardown() {
            // delete instance
            delete &getInstance();
        }
        // *****************
    private:
        // *** Singleton ***
        SleepSafeSingleton(const SleepSafeSingleton&) = delete;             //prevent copy
        SleepSafeSingleton& operator=(const SleepSafeSingleton&) = delete;  //prevent assign
        // *****************
        // *** Sleep Safe ***
        explicit SleepSafeSingleton(const char* namespaceName) {            //prevent direct initialise, attempt to init from restore.
            // init heap vars
            preferences = new Preferences();
            data = new JsonObject();
            // start preferences and load
            preferences->begin(namespaceName, true); // true: Flash, false: RTC Mem
            _store2mem();

            Serial.println("Singleton Initialised.");
        };
        virtual ~SleepSafeSingleton() {
            // store to preferences
            _mem2store();
            preferences->end();
            // delete heap vars
            delete data;
            delete preferences;

            Serial.print("Singleton Toredown.");
        }
        void _mem2store() {
            /* dont think this is needed as data is already type JsonObject
            // write data object to JsonObject => Document
            DynamicJsonDocument doc(JSON_OBJECT_SIZE(*data)); //! this may have another method to get size, JSON_OBJECT_SIZE is meant for types i think
            JsonObject obj = doc.to<JsonObject>();
            obj["data"] = *data;
            serializeJson(doc, *preferences);
            */
            String jsonStr;
            serializeJson(*data, jsonStr);
            preferences->putString("data", jsonStr);
        }
        void _store2mem() {
            // DynamicJsonDocument doc(JSON_OBJECT_SIZE(*data));
            DynamicJsonDocument doc(ESP.getMaxAllocHeap());
            String jsonStr = preferences->getString("data", "{}");
            DeserializationError error = deserializeJson(doc, jsonStr);
            if (error || jsonStr == "{}") {
                // could not load values from store
                Serial.println("No previous data found, loading defaults.");
                _default2mem();
            } else {
                // load values from store
                /* dont think this is needed as data is already type JsonObject
                JsonObject obj = doc.as<JsonObject>();
                data = obj["data"];
                */
                Serial.println("Loading previous data from Flash/RTC.");
                *data = doc.as<JsonObject>();
            }
        }
        virtual void _default2mem() {
            // set default values, this should be done in the derived classes.
            *data = JsonObject();
            //! testing
            *data["t"] = "Not overriden.";
        }
        // ***
        Preferences* preferences;
        JsonObject* data;
        // ***
        // ******************
};



// example
class MyDerivedClass : public SleepSafeSingleton<MyDerivedClass>("MyClass") {
    public:
        // func here
        MyDerivedClass() {
            // init heap vars.
            // populate data
            Serial.println("Derived class init.");
        }
        ~MyDerivedClass() {
            // delete heap vars.
            Serial.println("Derived class teardown.");
        }
        void printData() {
            serializeJson(*data, Serial);
            Serial.println();
        }
    private:
        void _default2mem() override {
            *data = JsonObject();
            *data["t1"] = 1;
            *data["t2"] = 2;
        }
};

MyDerivedClass& test = MyDerivedClass::getInstance();
test.printData();
//sleep
MyDerivedClass& test = MyDerivedClass::getInstance();
test.printData();