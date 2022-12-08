/*
 * MIT License
 * Copyright (c) 2021 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __STORAGE_SERVICE_H
#define __STORAGE_SERVICE_H
#include "ArduinoJson.h"
#include "lvgl.h"
#include <algorithm>
#include "../DataCenter/DataCenterLog.h"
#include <stdint.h>
#include <vector>
class FileWrapper
{
public:
    FileWrapper(const char* path, lv_fs_mode_t mode)
    {
        memset(&file, 0, sizeof(file));
        fs_res = lv_fs_open(&file, path, mode);
    }

    ~FileWrapper()
    {
        lv_fs_close(&file);
    }

    uint8_t read()
    {
        uint8_t data = 0;
        readBytes(&data, 1);
        return data;
    }

    size_t readBytes(void* buffer, size_t length)
    {
        uint32_t br = 0;
        lv_fs_read(&file, buffer, (uint32_t)length, &br);
        return br;
    }

    size_t write(uint8_t c)
    {
        return write(&c, 1);
    }

    size_t write(const uint8_t* s, size_t n)
    {
        uint32_t bw = 0;
        lv_fs_write(&file, s, (uint32_t)n, &bw);
        return bw;
    }

    operator bool()
    {
        return fs_res == LV_FS_RES_OK;
    };

private:
    lv_fs_res_t fs_res;
    lv_fs_file_t file;
};
class StorageService
{
public:
    typedef enum
    {
        TYPE_UNKNOW,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_DOUBLE,
        TYPE_STRING
    } DataType_t;

public:
    StorageService();
    ~StorageService();

    bool Add(const char* key, void* value, uint16_t size, DataType_t type);
    bool Remove(const char* key);
    bool SaveFile(const char* backupPath = nullptr);
    bool LoadFile(const char *FilePath);
    bool LoadFile(uint32_t bufferSize);
    void Analyze();

private:
    typedef struct
    {
        const char* key;
        void* value;
        uint16_t size;
        DataType_t type;
    } Node_t;

private:
    const char* FilePath;
    std::vector<Node_t*> NodePool;
    uint32_t BufferSize;

private:
    Node_t* SearchNode(const char* key);    
};

#endif
