#include "system.h"
#include "network.h"
#include "post.h"
#include "interrupt.h"

void RingTopologyTest()
{
    PacketHeader outPacketHeader, inPacketHeader;
    MailHeader outMailHeader, inMailHeader;

    char data = 0;
    char *ack = "Got it!";
    char receptionBuffer[MaxMailSize];

    if (!netAddr)
    {
        outPacketHeader.to = ++data;
        outMailHeader.to = 0;
        outMailHeader.from = 1;
        outMailHeader.length = 2;
        postOffice->Send(outPacketHeader, outMailHeader, data);
    }

    postOffice->Receive(1, &inPacketHeader, receptionBuffer);
    
    outPacketHeader.to = inPacketHeader.from;
    outMailHeader.to = inMailHeader.from;
    outMailHeader.length = strlen(ack) + 1;
    postOffice->Send(outPacketHeader, outMailHeader, ack);
    
}