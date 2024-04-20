#pragma once

#ifdef QT_DEBUG

#define AV_ERR(res) char buff[256]; \
                    av_strerror(res, buff, 256);

#endif
