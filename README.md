# MXL-PlugY
___

## General Information

MXL-PlugY is a project that based on [PlugY, The Survival Kit v11.02](https://github.com/ChaosMarc/PlugY/releases/tag/11.02) by Yohann Nicolas and designed to work specifically with Median XL Sigma, a popular total conversion mod for Diablo II.
Only certain *stable* versions of MXL Sigma that correspond to major patches are supported, see the [table](#supported-versions) below.

**MXL-PlugY is incompatible with any version of MXL Sigma starting from 2.0 onwards.**

## Available Features

The most iconic PlugY features are fully available such as
* Unlimited personal and shared stash
* Attributes and skill points reallocation
* Comprehensive character stats information

Furthermore, there are also a few extra qol features
* Restored /players command (*ActiveCommands*)
* Gamble stock refresh (*EnableGambleRefresh*)
* Legacy boss lifebar (*ForceLegacyBossLifebar*)
* No respawn timer (*RemoveRespawnCooldown*)

see [PlugY.ini](PlugYInstaller/PlugY.ini) and [PlugY Readme](PlugYInstaller/PlugY_The_Survival_Kit_-_Readme.txt) for additional information.

## Supported Versions

Core module D2Sigma.dll has to be one of the following versions

| Version      | SHA-1                                      |
|--------------|:------------------------------------------:|
| 1.0.4/1.0.4b | `8FDF7986D5B29EF799EBABCAFD681F6A15E7480D` |
| 1.1.4        | `BAA0A70544FE92C7751CF8407ED11ABDF1A01571` |
| 1.2.3        | `1E8B32D2EE7467623B248889A68C6A921B487CBF` |
| 1.3.4        | `62F0088A27A75D646C53BC33CF153C45CCAB8A59` |
| 1.4.0/1.4.1  | `ECD522018E7CC3A8AAC9B84821682DAD5A091012` |
| 1.5.3        | `F9475C0559E2EC8A314AD38A71548FCF23249564` |
| 1.6.2/1.6.3  | `8BE36190CEE438210D863752D5AC8E7646A33764` |
| 1.7.0-1.7.4  | `AC337812FDB2DD8E79E30E0A1AD3DC98FAC57E5B` |
| 1.8.0        | `2B462099D2884902452C87F3B03668BE651BB167` |

## Installation

Consult [PlugY Readme](PlugYInstaller/PlugY_The_Survival_Kit_-_Readme.txt) for details.

## Building from Source

Microsoft Windows SDK and MSVC CRT are required to build the main module (PlugY.dll), for that reason it is highly recommended to use Visual Studio (preferably VS2017 or newer).
However this project does not utilize any modern C++ features, technically it should be fully compatible with the C++03 standard for the very least.
