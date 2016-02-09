#ifndef GETOPTION_H
#define GETOPTION_H

#include <QMap>
#include <iostream>
#include <QTextStream>

class OptionItem
{
public:
    OptionItem();
    OptionItem(const QString &_op, const QString &_description, const QString &_defaultVaule);
    OptionItem(const QString &_op, const QString &_description); // if leave defaultValue blank, it will be regarded as a flag option

    void    enableOptionFlag();
    bool    getOptionFlag();
    bool    isFlagOption();
    void    setOptionValue(const QString _value);
    QString getOptionValue();
    QString getOptionString();
    QString getOptionDescription();
private:
    QString op;
    QString description;
    QString defaultValue;
    bool flagOption; // if it is a flag option that not accept any param
    bool flag; //actual flag
    QString value; //actual value
};

class GetOption
{
public:
    GetOption();
    void addOption(OptionItem &item);
    void addOption(const QString &op,const QString &description); // flag option
    void addOption(const QString &op,const QString &description,const QString &defaultValue); // with value

    void process(int argc, char** argv);

    bool isOptionKeyExists(const QString &op);

    QString getOptionValue(const QString &op) const;
    bool    getOptionFlag(const QString &op) const;
    bool    isFlagOption(const QString &op);
    OptionItem & getItem(const QString &op);

    void    showHelpOption();
    void    showVersionInfo();

    void    setDescriptionOption(const QString &des);
    void    setVersionInfo(const QString &_version);
private:
    QMap<QString, OptionItem> optionMap;
    QString appDescription;
    QString version;
    static const int SPACE_LETTERS = 20;
};

#endif // GETOPTION_H
