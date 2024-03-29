/*
 * Channel.h
 *
 *  Created on: Jan 15, 2015
 *      Author: fds
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <iostream>

class Channel {
private:
    float *data;
    int width;
    int height;
    int nChns;
public:
    Channel();
    virtual ~Channel();
    void PrintChanneldata() const;
    void PrintChanneldata(int w, int h) const;
    int getWidth() const;
    int getHeight() const;
    int getnChns() const;
    float *getData() const;

    float getLambda() {
        return 0.0;
    }

protected:
    void setChanneldata(float* data, int width, int height, int nChns);
};

#endif /* CHANNEL_H_ */
