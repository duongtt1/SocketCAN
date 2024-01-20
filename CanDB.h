#ifndef CANDB_H
#define CANDB_H

#include "Common/Common.h"
#include <map>
#include <string>
#include <memory>
#include <mutex>

using namespace std::chrono;

/**
 * @brief Singleton class representing a CAN database.
 */
class CanDB {
private:
    std::map<canid_t, std::shared_ptr<ICAN_MSG>> rxMessages;
    std::map<canid_t, std::shared_ptr<ICAN_MSG>> txMessages;
    std::map<canid_t, steady_clock::time_point> periodTimeToSendMsg;
    std::map<canid_t, steady_clock::time_point> periodTimeToRecvMsg;

    static std::unique_ptr<CanDB> instance;
    static std::mutex mutex;
    static bool isInitializedDb;

    CanDB() {}

public:
    ~CanDB() {}

    /**
     * @brief Gets the singleton instance of CanDB.
     * @return Reference to the CanDB instance.
     */
    static CanDB& getInstance();

    // Delete the copy constructor and assignment operator
    CanDB(const CanDB&) = delete;
    CanDB& operator=(const CanDB&) = delete;

    /**
     * @brief Adds a receive CAN message to the database.
     * 
     * @param id CAN identifier.
     * 
     * @param msg Pointer to the receive CAN message.
     */
    void setRxCanMessage(canid_t id, const std::shared_ptr<ICAN_MSG>& msg);

    /**
     * @brief Adds a transmit CAN message to the database.
     * 
     * @param id CAN identifier.
     * 
     * @param msg Pointer to the transmit CAN message.
     */
    void setTxCanMessage(canid_t id, const std::shared_ptr<ICAN_MSG>& msg);

    /**
     * @brief Retrieves a receive CAN message from the database.
     * 
     * @param id CAN identifier.
     * 
     * @return Shared pointer to the receive CAN message.
     */
    std::shared_ptr<ICAN_MSG> getRxCanMessageByID(canid_t id);

    /**
     * @brief Retrieves a transmit CAN message from the database.
     * 
     * @param id CAN identifier.
     * 
     * @return Shared pointer to the transmit CAN message.
     */
    std::shared_ptr<ICAN_MSG> getTxCanMessageByID(canid_t id);

    /**
     * @brief Gets the number of receive CAN messages in the database.
     * @return Number of receive CAN messages.
     */
    uint16_t getNumberOfRxMessages() const;

    /**
     * @brief Gets the number of transmit CAN messages in the database.
     * @return Number of transmit CAN messages.
     */
    uint16_t getNumberOfTxMessages() const;

    /**
     * @brief Gets a copy of the transmit CAN messages database.
     * @return Map of transmit CAN messages.
     */
    std::map<canid_t, std::shared_ptr<ICAN_MSG>> getTxMessages();

    /**
     * @brief Gets a copy of the receive CAN messages database.
     * @return Map of receive CAN messages.
     */
    std::map<canid_t, std::shared_ptr<ICAN_MSG>> getRxMessages();
};

#endif
