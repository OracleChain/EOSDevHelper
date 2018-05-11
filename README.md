# About/关于

**EOSDevHelper** is a cross-platform EOS PC wallet tool,which is developed by [OracleChain.io](https://oraclechain.io).

**EOSDevHelper**是一个跨平台PC端EOS钱包工具，由[OracleChain团队](https://oraclechain.io)研发。

-------------------------------

# Menu/目录
* [Overview  简介](#1)
* [ENVIRONMENT  开发和使用环境](#2)
* [Wallet Functions  钱包功能](#3)
* [Account Functions  账号功能](#4)
* [Contract Functions  合约功能](#5)
* [Transaction Checker  交易核查](#6)
* [About OracleChain  有关欧链](#7)
* [LICENSE  版权](#8)

------------------------------

<h2 id="1">Overview/简介</h2>

&emsp;&emsp;The EOSDevHelper program is a light weighted wallet build on EOSIO ecology. It provides private basic key store with encryptions, signature, validation functions, and etc. Developer can use this tool connect the EOSIO testnet, and test the wallet/account/contracts interface in a more convenient way.

&emsp;&emsp;EOS开发助手是一个建立在EOSIO生态上的钱包工具。它提供了基础的钱包功能，包含了私钥加密管理、签名校验模块。开发者可以使用EOS开发助手连接EOSIO网络，更便捷地对钱包、账号、合约接口等进行开发和测试。


------------------------------
<h2 id="2">ENVIRONMENT/开发和使用环境</h2>

**ENVIRONMENT/开发运行环境：**

&emsp;MACOS、WINDOWS、UBUNTU

&emsp;1.install [QT](https://www.qt.io/download) >= 5.8

&emsp;2.download our git repository.

&emsp;3.open QT and choose "Open Project", select the "src/EOSDevHelper.pro" and open

&emsp;4.select Build from QT menu and Run it.


**DOWNLOAD & TRY/下载试用**

|Version|MD5|
|------|---|
|[WINDOWS_v1.0(compliant with DAWN-2018-04-27-ALPHA)](https://github.com/OracleChain/EOSDevHelper/files/1937575/EOSDevHelper_Windows_Release.zip)|aa0539bddc17007fb47938b5ff9c80c6|
|[MAC_OSX_v1.0(compliant with DAWN-2018-04-27-ALPHA)](https://github.com/OracleChain/EOSDevHelper/releases/download/1.0/EOSDevHelper_OSX_Release.zip)|65e580e2372646a99af9a4f0d14e4340|


**DEPENDENCYS/依赖库:**

&emsp;1.our ECDSA is based on micro-ecc

&emsp;&emsp;椭圆曲线算法实现来自micro-ecc

&emsp;&emsp;https://github.com/kmackay/micro-ecc

&emsp;2.We build a Publicly Verifiable Secret Sharing on secp256k1 which is published by Schoenmakers on crypto99 conference.

&emsp;&emsp;我们在椭圆曲线secp256k1上实现了Schoenmakers在crypto99上提出的Publicly Verifiable Secret Sharing

&emsp;&emsp;https://github.com/songgeng87/PubliclyVerifiableSecretSharing


**Any questions pls join our official Telegram Group below/相关问题反馈，请加欧链官方Telegram群组**

&emsp;中文群：https://t.me/OracleChainChatCN

&emsp;ENGLISH GROUP：https://t.me/OracleChainChat

------------------------------

<h2 id="3">Wallet Functions/钱包功能</h2>

### Settings/设置
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/setting.PNG)

### Create a wallet/创建钱包
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/wallet.PNG)

### Create & import a private key/创建和导入私钥
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/ImportKey.PNG)

### Lock or Unlock a wallet/锁定和解锁钱包
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/UnlockWallet.PNG)


<h2 id="4">账号功能/Account Functions</h2>

### Create account/创建账号
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/account.PNG)

### Get Account Status/账号状态
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/GetAccount.PNG)

### Get Account History/账号交易历史
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/GetTransaction.PNG)

### Get Account Servants/账号子集
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/GetServants.PNG)



<h2 id="5">Contract Functions/合约功能</h2>

### Send a transaction/发起合约调用
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/Push.PNG)

### Get data table of a contract/查看合约数据表
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/GetTable.PNG)


<h2 id="6">Transaction Checker/交易核查</h2>

### 根据交易原文和签名，验证链上某交易是否成立/Using the action message with its signature,we can check if a transaction is signed by the owener of a given public key. 
![](https://github.com/OracleChain/EOSDevHelper/raw/master/screenshots/Checker.PNG)



------------------------------
<h2 id="7">About OracleChain/有关欧链</h2>

As the world’s first application built on an EOS ecosphere, OracleChain needs to meet the demands of the Oracle (oracle machine) ecosystem by efficiently linking blockchain technology services with various real-life scenarios, thereby delving into this immense tens of billions of dollars valuation market.

OracleChain（欧链）作为全球第一个直面区块链生态Oracle（预言机）需求的基础应用，将区块链技术服务和现实生活中的多种需求场景直接高效对接，深耕这个百亿美金估值的巨大市场。

As a decentralized Oracle technology platform based on the EOS platform, the autonomous Proof-of-Reputation & Deposit mechanism is adopted and used as a fundamental service for other blockchain applications.In addition to Oracle services that provide real-world data to the blockchain, Oracle services that provide cross-chain data are also offered. Given that OracleChain can accomplish the functions of several prediction market applications, such as Augur and Gnosis, OracleChain can also support smart contract businesses that require high-frequency access to outside data in certain scenarios, such as Robo-Advisor.

OracleChain是一个多区块链的去中心化Oracle技术平台，采用自主的PoRD机制，将现实世界数据引入区块链，并将此作为基础设施为其他区块链应用提供服务。
OracleChain将在区块链内提供现实世界数据的Oracle服务，同时还可以提供跨链数据的Oracle服务。基于OracleChain除了能实现Augur、Gnosis等预测市场（Prediction Market）应用的功能之外，还能支撑对链外数据有更高频率访问需求的智能合约业务，比如智能投顾等场景。

OracleChain will nurture and serve those blockchain applications that change the real world. Our mission is to “Link Data, Link World,” with the aim of becoming the infrastructure linking the real world with the blockchain world.

OracleChain将改变当前区块链应用的开发模式，建立全新的生态圈，服务于真正能改变现实世界的区块链应用。

By achieving intra-chain and extra-chain data connectivity, we aspire to create a service provisioning platform that can most efficiently gain access to extra-chain data in the future blockchain world.

OracleChain的使命是“让世界与区块链互联”，立志成为链接现实世界与区块链世界的基础设施，通过把外部数据引入区块链来实现链内链外的数据互通，OracleChian将是未来区块链世界中最高效的获取链外数据的服务提供平台。

<h2 id="8">LICENSE/版权</h2>

**License**

Released under GNU/LGPL Version 3
