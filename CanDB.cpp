#include "CanDB.h"
#include "Common/CNF_CAN.h"

// Initialize static members
std::unique_ptr<CanDB> CanDB::instance;
std::mutex CanDB::mutex;
bool CanDB::isInitializedDb;

CanDB& CanDB::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!instance)
    {
        instance.reset(new CanDB);
    }

    // init Rx Tx for CanDB
    if (isInitializedDb == false)
    {
        CFGCAN::getInstance().initRx();
        CFGCAN::getInstance().initTx();
        isInitializedDb = true;
    }
    else
    {
        // Do nothing
    }


    return *instance;
}

void CanDB::setRxCanMessage(canid_t id, const std::shared_ptr<ICAN_MSG>& msg)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->rxMessages[id] = msg;
}

void CanDB::setTxCanMessage(canid_t id, const std::shared_ptr<ICAN_MSG>& msg)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->txMessages[id] = msg;
}

std::shared_ptr<ICAN_MSG> CanDB::getRxCanMessageByID(canid_t id)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = rxMessages.find(id);
    if (it != rxMessages.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<ICAN_MSG> CanDB::getTxCanMessageByID(canid_t id)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = txMessages.find(id);
    if (it != txMessages.end()) {
        return it->second;
    }
    return nullptr;
}

uint16_t CanDB::getNumberOfRxMessages() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return static_cast<uint16_t>(rxMessages.size());
}

uint16_t CanDB::getNumberOfTxMessages() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return static_cast<uint16_t>(txMessages.size());
}

std::map<canid_t, std::shared_ptr<ICAN_MSG>> CanDB::getTxMessages()
{
    return txMessages;
}


std::map<canid_t, std::shared_ptr<ICAN_MSG>> CanDB::getRxMessages()
{
    return rxMessages;
}