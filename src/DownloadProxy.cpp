//
// Created by dev on 2020/3/10.
//

#include "DownloadProxy.h"

DownloadProxy::DownloadProxy() {}

DownloadProxy *DownloadProxy::instance = nullptr;

DownloadProxy *DownloadProxy::getInstance() {

  if (instance == nullptr) {
    instance = new DownloadProxy();
  }
  return instance;
}
