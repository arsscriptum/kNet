/* Copyright 2010 Jukka Jyl�nki

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

/** @file MessageConnectionDialog.cpp
	@brief */

#include <QUiLoader>
#include <QFile>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>

#ifdef KNET_USE_BOOST
#include <boost/thread/thread.hpp>
#endif

#include "kNet/DebugMemoryLeakCheck.h"
#include "kNet/Network.h"
#include "kNet/UDPMessageConnection.h"

#include "kNet/qt/MessageConnectionDialog.h"
#include "kNet/qt/ui/ui_MessageConnectionDialog.h"

namespace kNet
{

const int dialogUpdateInterval = 200;

MessageConnectionDialog::MessageConnectionDialog(QWidget *parent, Ptr(MessageConnection) connection_)
:connection(connection_), QWidget(parent)
{
/*
	QUiLoader loader;
	QFile file("MessageConnectionDialog.ui");
	file.open(QFile::ReadOnly);
	QWidget *myWidget = loader.load(&file, this);
	file.close();
*/
	dialog = new Ui_MessageConnectionDialog;
	dialog->setupUi(this);

/*
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(myWidget);
	setLayout(layout);
*/
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(Update()));
	Update();

	this->setAttribute(Qt::WA_DeleteOnClose);
}

MessageConnectionDialog::~MessageConnectionDialog()
{
	delete dialog;
}

void MessageConnectionDialog::Update()
{
	if (!connection)
		return;

	Socket *socket = connection->GetSocket();

	std::stringstream ss;
	if (!socket)
		ss << "(null connection)";
	else
		ss << SocketTransportLayerToString(socket->TransportLayer()) << " connection to " << connection->RemoteEndPoint().ToString()
		   << " (from " << connection->LocalEndPoint().ToString() << ")";
	dialog->connectionLine->setText(ss.str().c_str());
	setWindowTitle(ss.str().c_str());

	dialog->statusLine->setText(ConnectionStateToString(connection->GetConnectionState()).c_str());
	dialog->outboundMessagesPending->setText(QString::number(connection->NumOutboundMessagesPending()));
	dialog->messageConnectionStatus->setText(connection->Connected() ? QString("Connected") : QString("") + 
		(connection->IsReadOpen() ? " Read-open" : "") + (connection->IsWriteOpen() ? "Write-open" : ""));
	if (!socket)
		dialog->socketStatus->setText("(null)");
	else
		dialog->socketStatus->setText(socket->Connected() ? QString("Connected") : QString("") + 
			(socket->IsReadOpen() ? " Read-open" : "") + (socket->IsWriteOpen() ? "Write-open" : ""));
	dialog->roundTripTime->setText(QString::number(connection->RoundTripTime()) + "ms");
	dialog->lastHeardTime->setText(QString::number(connection->LastHeardTime()) + "ms");
	dialog->datagramsIn->setText(QString::number(connection->PacketsInPerSec()) + "/sec");
	dialog->datagramsOut->setText(QString::number(connection->PacketsOutPerSec()) + "/sec");
	dialog->messagesIn->setText(QString::number(connection->MsgsInPerSec()) + "/sec");
	dialog->messagesOut->setText(QString::number(connection->MsgsOutPerSec()) + "/sec");
	dialog->bytesIn->setText(QString(FormatBytes(connection->BytesInPerSec()).c_str()) + "/sec");
	dialog->bytesOut->setText(QString(FormatBytes(connection->BytesOutPerSec()).c_str()) + "/sec");
	dialog->totalBytesReceived->setText(QString(FormatBytes(connection->BytesInTotal()).c_str()));
	dialog->totalBytesSent->setText(QString(FormatBytes(connection->BytesOutTotal()).c_str()));

	UDPMessageConnection *udpConnection = dynamic_cast<UDPMessageConnection *>(connection.ptr());
	QLabel *labels[] =
	{ 
		dialog->retransmissionTimeoutHeader, dialog->retransmissionTimeout,
		dialog->datagramSendRateHeader, dialog->datagramSendRate,
		dialog->smoothedRTTHeader, dialog->smoothedRTT,
		dialog->rttVariationHeader, dialog->rttVariation,
		dialog->outUnackedDatagramsHeader, dialog->outUnackedDatagrams,
		dialog->recvUnackedDatagramsHeader, dialog->recvUnackedDatagrams,
		dialog->packetLossCountHeader, dialog->packetLossCount,
		dialog->packetLossRateHeader, dialog->packetLossRate
	};
	for(int i = 0; i < sizeof(labels)/sizeof(labels[0]); ++i)
		labels[i]->setVisible(udpConnection != 0);

	if (udpConnection)
	{
		dialog->retransmissionTimeout->setText(QString::number(udpConnection->RetransmissionTimeout()) + "ms");
		dialog->datagramSendRate->setText(QString::number(udpConnection->DatagramSendRate()) + "/sec");
		dialog->smoothedRTT->setText(QString::number(udpConnection->SmoothedRtt()) + "ms");
		dialog->rttVariation->setText(QString::number(udpConnection->RttVariation()));
		dialog->outUnackedDatagrams->setText(QString::number(udpConnection->NumOutboundUnackedDatagrams()));
		dialog->recvUnackedDatagrams->setText(QString::number(udpConnection->NumReceivedUnackedDatagrams()));
		dialog->packetLossCount->setText(QString::number(udpConnection->PacketLossCount()));
		dialog->packetLossRate->setText(QString::number(udpConnection->PacketLossRate()));
	}
	updateTimer->start(dialogUpdateInterval);
}

} // ~kNet
