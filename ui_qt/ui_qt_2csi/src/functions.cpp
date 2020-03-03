#include "functions.h"
#include <QTextCodec>

QString TextToUTF8(const char *sr)
{
	QByteArray encodedString(sr);
 	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
 	QString Str;
 	Str = codec->toUnicode(encodedString);
 	return Str;
}
// place your code here
