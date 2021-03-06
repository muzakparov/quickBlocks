## [{NAME}]

***
**Note:** This tool is available through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
***

`chifra` is an command line tool that builds [QBlocks account monitors](../../monitors/README.md) through a series of interactive questions.

Given your answers to those questions (such as which Ethereum addresses to monitor, what to name those addresses, where to find ABI files for those addresses), `chifra` generates the C++ code for both the parsing library needed to monitor the addresses and the code to build an interactive transactional debugging/auditing program.

Prior to running the program, collect together three pieces of information for each address you wish to monitor. You may monitor as many addresses as you wish. The information you need per address is:

- an Ethereum address
- a name to assign to this address (you may pick up the name using [ethName](../../tools/ethName/README.md))
- the block number when this address was first used, or (if a smart contract) when it was deployed.

`Chifra` uses [makeClass](../makeClass/README.md) and [grabABI](../grabABI/README.md) to build the monitors.

Here is a screen shot of an interaction with Chifra building a smart contract monitor for a particular token.

<img src=docs/image.png>

[{USAGE_TABLE}]
[{FOOTER}]
