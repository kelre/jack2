/*
Copyright (C) 2001 Paul Davis
Copyright (C) 2008 Romain Moret at Grame

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __JackNetInterface__
#define __JackNetInterface__

#include "JackNetTool.h"

namespace Jack
{
    /**
    \Brief This class describes the basic Net Interface, used by both master and slave
    */

    class EXPORT JackNetInterface
    {
        protected:
            session_params_t fParams;
            JackNetSocket fSocket;
            char* fMulticastIP;
            uint fNSubProcess;

            //headers
            packet_header_t fTxHeader;
            packet_header_t fRxHeader;

            //network buffers
            char* fTxBuffer;
            char* fRxBuffer;
            char* fTxData;
            char* fRxData;

            //jack buffers
            NetMidiBuffer* fNetMidiCaptureBuffer;
            NetMidiBuffer* fNetMidiPlaybackBuffer;
            NetAudioBuffer* fNetAudioCaptureBuffer;
            NetAudioBuffer* fNetAudioPlaybackBuffer;

            //sizes
            int fAudioRxLen;
            int fAudioTxLen;
            int fPayloadSize;

            virtual void SetParams();

            //virtual methods : depends on the sub class master/slave
            virtual bool Init() = 0;

            virtual int SyncRecv() = 0;
            virtual int SyncSend() = 0;
            virtual int DataRecv() = 0;
            virtual int DataSend() = 0;

            virtual int Send ( size_t size, int flags ) = 0;
            virtual int Recv ( size_t size, int flags ) = 0;

            JackNetInterface()
            {}
            JackNetInterface ( const char* ip, int port );
            JackNetInterface ( session_params_t& params, JackNetSocket& socket, const char* multicast_ip );

        public:
            virtual ~JackNetInterface();
    };

    /**
    \Brief This class describes the Net Interface for slaves (NetDriver and NetAdapter)
    */

    class EXPORT JackNetSlaveInterface : public JackNetInterface
    {
        protected:
            bool Init();
            net_status_t GetNetMaster();
            net_status_t SendMasterStartSync();
            void SetParams();
            int SyncRecv();
            int SyncSend();
            int DataRecv();
            int DataSend();

            int Recv ( size_t size, int flags );
            int Send ( size_t size, int flags );

        public:
            JackNetSlaveInterface()
            {}
            JackNetSlaveInterface ( const char* ip, int port ) : JackNetInterface ( ip, port )
            {}
            ~JackNetSlaveInterface()
            {
                SocketAPIEnd();
            }
    };

    /**
    \Brief This class describes the Net Interface for masters (NetMaster)
    */

    class EXPORT JackNetMasterInterface : public JackNetInterface
    {
        protected:
            bool fRunning;

            bool Init();
            void SetParams();
            void Exit();
            int SyncRecv();
            int SyncSend();
            int DataRecv();
            int DataSend();

            int Send ( size_t size, int flags );
            int Recv ( size_t size, int flags );

        public:
            JackNetMasterInterface() : fRunning ( false )
            {}
            JackNetMasterInterface ( session_params_t& params, JackNetSocket& socket, const char* multicast_ip )
                    : JackNetInterface ( params, socket, multicast_ip )
            {}
            ~JackNetMasterInterface()
            {}
    };
}

#endif
