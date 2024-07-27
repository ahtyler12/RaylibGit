#include <string>
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "json/writer.h"
#pragma once

struct PlayerData
{
    std::string playerName = "Kupo";

private:
    std::string fileName = "./resources/PlayerData.json"; //Set as Private so that nothing can modify the location of this. Maybe should be protected instead but I'll keep for now
public:
    void LoadData()
    {       
        Json::Value pData;
        Json::Reader reader;
        std::ifstream pFile(fileName, std::ifstream::binary);
        reader.parse(pFile, pData);
        playerName = pData["Player Name"].asString();
        pFile.close();
    }

    void SaveData()
    {
        std::ofstream pFile(fileName);
        Json::Value pData;
        Json::StreamWriterBuilder builder;
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        pData["Player Name"] = playerName;

        writer->write(pData, &pFile);
        pFile.close();

    }
};
