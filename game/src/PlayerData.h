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
    std::string fileName = "PlayerData.txt";
public:
    void LoadData()
    {       
        Json::Value pData;
        Json::Reader reader;
        std::ifstream pFile("PlayerData.json", std::ifstream::binary);
        reader.parse(pFile, pData);
        std::cout << pData;

        pFile.close();
    }

    void SaveData()
    {
        std::ofstream pFile("PlayerData.json");
        Json::Value pData;
        Json::StreamWriterBuilder builder;
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(pData, &pFile);
        pFile.close();

    }
};
