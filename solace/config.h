/* compiler options */

#pragma once

#pragma optimize("gsy", on)

#pragma comment(lib, "msimg32.lib")

#pragma warning(disable : 4244) /* possible loss of data warning */

/*
 * coding style:
 * if a function does not require any parameters, make sure they do not accept them: int func(void);
 * open all functions and statements on the same line: int func(void) {
 * no spaces between operators and their opening/closing parenthesis: sizeof ( none of this bs )
 * when comparing return values with operators always place the function second: if (0 == func()) { or if (0 != func()) {
 * always comment lines in this style
 * during the time of an error, all functions return -1. test against them like this: if (func() < 0) {
 * consecutive values should always be defined in enum, use #define otherwise
 * tagging a struct or enum is not necessary, follow the closing with the name and ;
 */