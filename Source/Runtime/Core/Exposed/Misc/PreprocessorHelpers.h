// CopyRight FlareHorz Team. All Rights Reserved.

#pragma once

/** Turns a preprocessor token into a real string. */
#define FH_STRINGIZE(Token) FH_PRIVATE_STRINGIZE(Token)
#define FH_PRIVATE_STRINGIZE(Token) #Token

/** Concatenates two preprocessor tokens, performing macro expansion on them first. */
#define FH_JOIN(TokenA, TokenB) FH_PRIVATE_JOIN(TokenA, TokenB)
#define FH_PRIVATE_JOIN(TokenA, TokenB) TokenA##TokenB

/** Creates a string that can be used to include a header with the platform in its name, like "Prefix/PlatformName/PlatformNameSuffix.h". */
#define PLATFORM_HEADER(Suffix) FH_STRINGIZE(FH_JOIN(PLATFORM_NAME/PLATFORM_NAME, Suffix))
