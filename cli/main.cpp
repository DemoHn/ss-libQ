/*
 * Copyright (C) 2015-2016 Nigshoxiz
 */

/* This is the libssQ-cli main file.
 * It supplies a simple CLI tool to create a shadowsocks / SSR client.
 * Transparent proxy is supported also.
 */

#include <QCoreApplication>
#include <signal.h>
#include <QDebug>

#include <sslibQ>
#include "main.h"
#include "getoption.h"
#include "config.h"

#include "client.h"

static void onSIGINT_TREM(int sig)
{
    if(sig == SIGINT || sig == SIGTERM)
        qApp->quit();
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc,argv);

    app.setApplicationName("sslibQ-cli");
    app.setApplicationVersion("0.1");

    signal(SIGINT, onSIGINT_TREM);
    signal(SIGTERM, onSIGINT_TREM);

    /*set option*/
    GetOption option;
    option.setVersionInfo("v0.1");
    option.setDescriptionOption("Usage: sslibQ-cli.exe [options] ");
    option.addOption("c","denote the config file. default: config.json ","config.json");
    option.addOption("redir","set as redir mode");
    option.addOption("server","set as server mode");
    option.addOption("client", "set as client mode");
    option.addOption("proxy","set proxy type. {socks | http | g_http}. default: socks","socks");
    option.addOption("i","set input interface (only work on redir mode). default: eth0","eth0");
    option.addOption("o","set output interface.(only work on redir mode). default: wlan0");
    option.process(argc,argv);

    // read option and setup
    // be careful
    /* get config file name*/
    QString file = option.getOptionValue("c");

    CLI_MODE cli_mode = CLIENT;
    //PROXY_MODE proxy_mode = SOCKS;

    Config config(file);

    /*determine cli_mode*/
    if(option.getOptionFlag("client"))
        cli_mode = CLIENT;
    if(option.getOptionFlag("server"))
        cli_mode = SERVER;
    if(option.getOptionFlag("redir"))
        cli_mode = REDIR;


    if(cli_mode == CLIENT){
       Client client;
       //client.setup(config.getProfile());
       if(!client.start(config)){
           qDebug() << "start client FAILED!";
           return 1;
       }
    }else if(cli_mode == SERVER){

    }else if(cli_mode == REDIR){

    }else{
        // never happen, since default value of cli_mode is CLIENT
    }

    return app.exec();
}
