#include "abstractplayer.h"

AbstractPlayer::AbstractPlayer()
	: QDialog()
{
	ui.setupUi(this);
}

AbstractPlayer::~AbstractPlayer()
{

}

void AbstractPlayer::init(QString filePath)
{
	this->filePath = filePath;
	QByteArray ba = filePath.toLocal8Bit();
	const char *file = ba.data();
	capture = cvCaptureFromAVI(file);
}