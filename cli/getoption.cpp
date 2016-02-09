/**
 * GetOption Class
 *
 * This class parses parameters from the CLI program and shows options hint like
 * what QCommandLineParser do.
 *
 * NOTE: Why not directly use QCommandLineParser?
 * I. It requires Qt >= 5.2
 * II. (Most important) It cannot be shown normally in cmd.exe on Windows!!
 *
 * Create by Nigshoxiz
 * 2016.1.28
 */

#include "getoption.h"
#include <cstring>
#include <QDebug>

OptionItem::OptionItem()
{

}

//option item class
OptionItem::OptionItem(const QString &_op, const QString &_description)
{
    op = _op;
    description = _description;
    flagOption = true;
    flag = false;
    value = "";
}

OptionItem::OptionItem(const QString &_op, const QString &_description, const QString &_defaultValue)
{
    op = _op;
    description = _description;
    defaultValue= _defaultValue;
    flagOption = false;
    flag = false;
    value = "";
}

void OptionItem::enableOptionFlag()
{
    flag = true;
}

bool OptionItem::getOptionFlag()
{
   return flag;
}

bool OptionItem::isFlagOption()
{
    return flagOption;
}

void OptionItem::setOptionValue(const QString _value)
{
    value = _value;
}

QString OptionItem::getOptionValue()
{
    if(value == "")
    {
        return defaultValue;
    }
    else
    {
        return value;
    }
}

QString OptionItem::getOptionString()
{
    return op;
}

QString OptionItem::getOptionDescription()
{
    return description;
}

//getoption class
GetOption::GetOption()
{
    appDescription = "";
    version = "";
}

//add option, and store them into a map
void GetOption::addOption(OptionItem &item)
{
    QString index_str = item.getOptionString();
    optionMap.insert(index_str,item);
}

void GetOption::addOption(const QString &op, const QString &description)
{
    OptionItem item(op,description);
    optionMap.insert(op,item);
}

void GetOption::addOption(const QString &op, const QString &description, const QString &defaultValue)
{
    OptionItem item(op,description,defaultValue);
    optionMap.insert(op,item);
}

QString GetOption::getOptionValue(const QString &op) const
{
    OptionItem item(optionMap.value(op));
    return item.getOptionValue();
}

bool GetOption::getOptionFlag(const QString &op) const
{
    OptionItem _item;
    if(optionMap.find(op) == optionMap.end()) //not found
    {
        return false;
    }else{
        _item = (optionMap.value(op));
        return _item.getOptionFlag();
    }
}

bool GetOption::isOptionKeyExists(const QString &op)
{
    if(optionMap.find(op) == optionMap.end()) // not found
    {
        return false;
    }else{
        return true;
    }
}

bool GetOption::isFlagOption(const QString &op)
{
    OptionItem i = optionMap.value(op);
    return i.isFlagOption();
}

OptionItem & GetOption::getItem(const QString &op)
{
    return optionMap[op];
}

/* show help :'-h' */
void GetOption::showHelpOption()
{
    //first show appDescription
    std::cout << std::endl << appDescription.toUtf8().constData() << std::endl << std::endl;

    std::cout << "Options:  " << std::endl;

    QMap<QString, OptionItem>::const_iterator it;

    for(it = optionMap.constBegin();it != optionMap.constEnd(); ++it){
        std::string key_str = it.key().toUtf8().constData();

        if(key_str.length() > 1){
            key_str = "--" + key_str;
        }else{
            key_str = "-" + key_str;
        }
        OptionItem i = it.value();
        std::string des_str = i.getOptionDescription().toUtf8().constData();

        std::string space_str;
        space_str.assign(SPACE_LETTERS - key_str.length(),' ');

        std::cout << "  " << key_str << space_str << des_str << std::endl;
    }
}

void GetOption::showVersionInfo()
{
    std::cout << "Version: " << version.toUtf8().constData() << std::endl;
}

/* store data function*/
void GetOption::process(int argc, char **argv)
{

    //if no other input
    if(argc <= 1){
        showHelpOption();
        return ;
    }

    for(int i=0;i<argc;i++){
        std::string str = std::string(argv[i]);
        if(str.at(0) == '-' && str.length() > 1){
            // show version data
            if(str == "-v" || str == "--version"){
                showVersionInfo();
                return ;
            }else if(str == "-h" || str == "--help"){
                showHelpOption();
                return ;
            }
            str = str.substr(1,str.length());

            if(str.at(0) == '-' && str.length() > 1){
                str = str.substr(1,str.length());
            }

            QString s = QString::fromLocal8Bit(str.c_str());
            if(isOptionKeyExists(s)){
                if(isFlagOption(s) == true){ // flag option
                    getItem(s).enableOptionFlag();
                }else{ // set value
                    if(i < argc - 1){ // not the last item
                        std::string next_str = std::string(argv[i+1]);
                        if(next_str[0] != '-'){
                            getItem(s).setOptionValue(QString::fromLocal8Bit(next_str.c_str()));
                        }
                    }
                }
            }else{
                // just ignore it
            }
        }
    }
}

void GetOption::setDescriptionOption(const QString &des)
{
    appDescription = des;
}

void GetOption::setVersionInfo(const QString &_version)
{
    version = _version;
}
