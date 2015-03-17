#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

float getRedChannel(float y, float u, float v){
    float px = 1.164*(y - 16) + 1.596*(v - 128);
    if(px < 0) return 0;
    if(px > 255) return 255;
    return px;
}
float getGreenChannel(float y, float u, float v){
    float px = 1.164*(y - 16) - 0.813*(v - 128) - 0.391*(u - 128);
    if(px < 0) return 0;
    if(px > 255) return 255;
    return px;
}
float getBlueChannel(float y, float u, float v){
    float px = 1.164*(y - 16) + 2.018*(u - 128);
    if(px < 0) return 0;
    if(px > 255) return 255;
    return px;
}



void MainWindow::pass(HRESULT hr){
    //std::cout << ((hr == S_OK) ? "Pass" : "Fail") << std::endl;
    //if(hr == S_OK) std::cout << "S_OK" << std::endl;
    if (FAILED(hr))
        exit(1);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    HRESULT hr = S_OK;

    // ---------------------- INIT
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    MFStartup(MF_VERSION);

    // ---------------------- DEVICE DETECTION
    UINT32 count;
    IMFActivate **ppDevices = NULL;
    IMFAttributes *pAttributes = NULL;
    IMFMediaSource *pMSource = NULL;
    hr = MFCreateAttributes(&pAttributes, 1); pass(hr);
    hr = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID); pass(hr);
    hr = MFEnumDeviceSources(pAttributes, &ppDevices, &count); pass(hr);
    if (count > 0){
        hr = ppDevices[0]->ActivateObject(IID_PPV_ARGS(&pMSource)); pass(hr);
    }
    else{
        hr = MF_E_NOT_FOUND; pass(hr);
    }
    pAttributes->Release();
    for(UINT32 i=0; i < count; i++)
        ppDevices[i]->Release();
    //delete ppDevices;

    //----------------------- SOURCE READER
    hr = MFCreateSourceReaderFromMediaSource(pMSource, NULL, &pReader); pass(hr);
    pMSource->Release();

    //----------------------- READ
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;

    // ---------------------- SHUTDONW
    MFShutdown();
    CoUninitialize();
}

void MainWindow::updateFrame(){
    HRESULT hr = S_OK;
    DWORD streamIndex, flags;
    LONGLONG tmStamp;
    IMFSample *pSample = NULL;

    while (pSample == NULL){
        hr = pReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, &streamIndex, &flags, &tmStamp, &pSample); pass(hr);
    }

    IMFMediaBuffer *pBuffer;
    hr = pSample->GetBufferByIndex(0, &pBuffer); pass(hr);
    DWORD bSize;
    BYTE *frame;
    hr = pBuffer->Lock(&frame, NULL, &bSize);

    // ---- YUV RGB Conversion
    BYTE *rgbFrame = new BYTE[(bSize/2)*3];
    for(DWORD i = 0; i< bSize/2; i+=2){
        int y  = (frame[2*i])     + ui->ySlider->value();
        int u  = (frame[2*i + 1]) + ui->uSlider->value();
        int y2 = (frame[2*i + 2]) + ui->ySlider->value();
        int v  = (frame[2*i + 3]) + ui->vSlider->value();

        rgbFrame[3*i + 0] = (int)getRedChannel  (y, u, v);
        rgbFrame[3*i + 1] = (int)getGreenChannel(y, u, v);
        rgbFrame[3*i + 2] = (int)getBlueChannel (y, u, v);

        rgbFrame[3*i + 3] = (int)getRedChannel  (y2, u, v);
        rgbFrame[3*i + 4] = (int)getGreenChannel(y2, u, v);
        rgbFrame[3*i + 5] = (int)getBlueChannel (y2, u, v);
    }

    QImage img(rgbFrame, 640, 480, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
    pBuffer->Unlock();
    pBuffer->Release();
    pSample->Release();
    delete rgbFrame;
}

