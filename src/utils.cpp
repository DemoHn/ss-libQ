#include "utils.h"

// parse ipv4 address
QString Utils::parseIP(QByteArray &ip_addr)
{
    if(ip_addr.size() != 4){
        return QString("0.0.0.0");
    }

    QString str = QString("%1.%2.%3.%4").arg(static_cast<uchar>(ip_addr.at(0)), 0, 10)
                                 .arg(static_cast<uchar>(ip_addr.at(1)), 0, 10)
                                 .arg(static_cast<uchar>(ip_addr.at(2)), 0, 10)
                                 .arg(static_cast<uchar>(ip_addr.at(3)), 0, 10);
    return str;
}


QByteArray Utils::pack(const char * fmt, ...)
{
    QByteArray rtn_buf(0);
    uchar *cs = new uchar[4];
    const char *str = NULL;
    va_list args;

    va_start(args,fmt);

    while(*fmt != '\0')
    {
        if(*fmt == 'B'){
            uchar byt = va_arg(args, uchar);
            qToBigEndian(byt,cs);
            rtn_buf.append(reinterpret_cast<const char *>(cs), 1);
        }

        if(*fmt == 'H'){
            ushort dbyt = va_arg(args, ushort);
            qToBigEndian(dbyt, cs);
            rtn_buf.append(reinterpret_cast<const char*>(cs), 2);
        }

        if(*fmt == 'X'){
            str = va_arg(args, const char*);
            rtn_buf.append(str);
        }

        if(*fmt == 'x'){
            QByteArray new_buf(va_arg(args, QByteArray));
            rtn_buf.append(new_buf);
        }
        if(*fmt == 'I'){
            uint qbyt = va_arg(args, uint);
            qToBigEndian(qbyt, cs);
            rtn_buf.append(reinterpret_cast<const char*>(cs), 4);
        }
        ++fmt;
   }

    delete [] cs;
    cs = NULL;
    va_end(args);
    return rtn_buf;
}


/*
unpack:
B : 1 bit
H : 2 bit
I : 4 bit
[<num>] : <num> bit. e.g. : [5] represents for 5 bit

[&<num>] : its length is determined by the value of the <num>th variable.
e.g. : "HBB[&3]" <--> "HBB[2]" if the parsing data is (00 01 07 02 05 03)

NOTICE : the referenced data is restricted before it. And buffer type is not counted.

e.g. :
"HB[2]I[&3]" --> refers to the value of "I" format data. "[2]" is not counted when calculating the order.
*/

unsigned int Utils::unpack(const char * fmt,const QByteArray &buf, ...)
{
    va_list args;
    va_start(args, buf);
    char * dat = const_cast<char*>(buf.data());
    int offset = 0;
    QList<qint32> arg_list;

    int var_count = 0;
    while(*fmt != '\0')
    {
        if(*fmt == 'B'){
            char* byt = va_arg(args,char*);
            // set value
            *byt = *(dat + offset);
            offset += 1;
            var_count += 1;
            arg_list << *byt;
        }

        if(*fmt == 'H'){
            qint16* dbyt = va_arg(args, qint16*);
            qint16  val  = qFromBigEndian<qint16>(reinterpret_cast<const uchar*>(dat + offset));

            *dbyt = val;
            offset += 2;
            var_count += 1;
            arg_list << *dbyt;
        }

        if(*fmt == 'I'){
            qint32* qbyt = va_arg(args,qint32*);
            qint32   val = qFromBigEndian<qint32>(reinterpret_cast<const uchar*>(dat+offset));

            *qbyt = val;
            offset += 4;
            var_count += 1;
            arg_list << *qbyt;
        }

        bool ref_flag = false;
        //parse [N]
        if(*fmt == '['){
            int num = 0;
            fmt += 1;

            if( *(fmt+1) == '&' ){
                ref_flag = true;
                fmt += 1;
            }

            while(*fmt != ']'){
                num *= 10;

                if(*fmt >= '0' && *fmt <= '9'){
                    num += (*fmt - '0');
                }
                ++fmt;
            }

            QByteArray* buf_ptr = va_arg(args,QByteArray*);

            if(ref_flag == true)
            {
                if(num > 0 && num <= var_count){
                    num = arg_list.at(num - 1);
                }
            }
            buf_ptr->resize(0);
            buf_ptr->append((dat+offset), num);

            offset += num;
        }
        ++fmt;
    }

    va_end(args);
    return offset;
}

unsigned int Utils::unpack(const char * fmt, char * data, ...)
{
    va_list args;
    va_start(args, data);
    char * dat = data;
    int offset = 0;
    QList<qint32> arg_list;

    int var_count = 0;
    while(*fmt != '\0')
    {
        if(*fmt == 'B'){
            char* byt = va_arg(args,char*);
            // set value
            *byt = *(dat + offset);
            offset += 1;
            var_count += 1;
            arg_list << *byt;
        }

        if(*fmt == 'H'){
            qint16* dbyt = va_arg(args, qint16*);
            qint16  val  = qFromBigEndian<qint16>(reinterpret_cast<const uchar*>(dat + offset));

            *dbyt = val;
            offset += 2;
            var_count += 1;
            arg_list << *dbyt;
        }

        if(*fmt == 'I'){
            qint32* qbyt = va_arg(args,qint32*);
            qint32   val = qFromBigEndian<qint32>(reinterpret_cast<const uchar*>(dat+offset));

            *qbyt = val;
            offset += 4;
            var_count += 1;
            arg_list << *qbyt;
        }

        bool ref_flag = false;
        //parse [N]
        if(*fmt == '['){
            int num = 0;
            fmt += 1;

            if( *fmt == '&' ){
                ref_flag = true;
                fmt += 1;
            }

            while(*fmt != ']'){
                num *= 10;

                if(*fmt >= '0' && *fmt <= '9'){
                    num += (*fmt - '0');
                }
                ++fmt;
            }

            QByteArray* buf_ptr = va_arg(args,QByteArray*);

            if(ref_flag == true)
            {
                if(num > 0 && num <= var_count){
                    num = arg_list.at(num - 1);
                }
            }
            buf_ptr->resize(0);
            buf_ptr->append((dat+offset), num);

            offset += num;
        }
        ++fmt;
    }

    arg_list.clear();
    va_end(args);
    return offset;
}
