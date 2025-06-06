/*
 * -------------------------------------------------------------------
 * CircuitSetup.us Time Circuits Display
 * (C) 2021-2022 John deGlavina https://circuitsetup.us
 * (C) 2022-2025 Thomas Winischhofer (A10001986)
 * https://github.com/realA10001986/Time-Circuits-Display
 * https://tcd.out-a-ti.me
 *
 * Font for TC LED segment displays
 *
 * -------------------------------------------------------------------
 * License: MIT NON-AI
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the 
 * Software, and to permit persons to whom the Software is furnished to 
 * do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * In addition, the following restrictions apply:
 * 
 * 1. The Software and any modifications made to it may not be used 
 * for the purpose of training or improving machine learning algorithms, 
 * including but not limited to artificial intelligence, natural 
 * language processing, or data mining. This condition applies to any 
 * derivatives, modifications, or updates based on the Software code. 
 * Any usage of the Software in an AI-training dataset is considered a 
 * breach of this License.
 *
 * 2. The Software may not be included in any dataset used for 
 * training or improving machine learning algorithms, including but 
 * not limited to artificial intelligence, natural language processing, 
 * or data mining.
 *
 * 3. Any person or organization found to be in violation of these 
 * restrictions will be subject to legal action and may be held liable 
 * for any damages resulting from such use.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _TC_FONT_H
#define _TC_FONT_H

#ifndef IS_ACAR_DISPLAY
static const uint16_t alphaChars[127-31-1] = {
    0b0000000000000000,  // <space>
    0b0000000000000110,  // !
    0b0000001000100000,  // "
    0b0001001011001110,  // #
    0b0001001011101101,  // $
    0b0000110000100100,  // %
    0b0010001101011101,  // &
    0b0000010000000000,  // '
    0b0010010000000000,  // (
    0b0000100100000000,  // )
    0b0011111111000000,  // *
    0b0001001011000000,  // +
    0b0000100000000000,  // ,
    0b0000000011000000,  // -
    0b0000000000001000,  // .
    0b0000110000000000,  // /
    0b0000000000111111,  // 0   [0b0000110000111111]
    0b0000000000000110,  // 1
    0b0000000011011011,  // 2
    0b0000000011001111,  // 3   [0b0000000010001111]
    0b0000000011100110,  // 4
    0b0000000011101101,  // 5   [0b0010000001101001]
    0b0000000011111100,  // 6   [0b0000000011111101]
    0b0000000000000111,  // 7
    0b0000000011111111,  // 8
    0b0000000011100111,  // 9   [0b0000000011101111]
    0b0001001000000000,  // :
    0b0000101000000000,  // ;
    0b0010010000000000,  // <
    0b0000000011001000,  // =
    0b0000100100000000,  // >
    0b0001000010000011,  // ?
    0b0000001010111011,  // @
    0b0000000011110111,  // A
    0b0001001010001111,  // B
    0b0000000000111001,  // C
    0b0001001000001111,  // D
    0b0000000011111001,  // E
    0b0000000001110001,  // F
    0b0000000010111101,  // G
    0b0000000011110110,  // H
    0b0001001000001001,  // I
    0b0000000000011110,  // J
    0b0010010001110000,  // K
    0b0000000000111000,  // L
    0b0000001000110111,  // M
    0b0010000100110110,  // N
    0b0000000000111111,  // O
    0b0000000011110011,  // P
    0b0010000000111111,  // Q
    0b0010000011110011,  // R
    0b0000000011101101,  // S
    0b0001001000000001,  // T
    0b0000000000111110,  // U
    0b0000110000110000,  // V
    0b0010100000110110,  // W
    0b0010110100000000,  // X
    0b0000000011101110,  // Y
    0b0000110000001001,  // Z
    0b0000000000111001,  // [
    0b0010000100000000,  // backslash
    0b0000000000001111,  // ]
    0b0000110000000011,  // ^
    0b0000000000001000,  // _
    0b0000000100000000,  // `
    0b0001000001011000,  // a
    0b0010000001111000,  // b
    0b0000000011011000,  // c
    0b0000100010001110,  // d
    0b0000100001011000,  // e
    0b0000000001110001,  // f
    0b0000010010001110,  // g
    0b0001000001110000,  // h
    0b0001000000000000,  // i
    0b0000000000001110,  // j
    0b0011011000000000,  // k
    0b0000000000110000,  // l
    0b0001000011010100,  // m
    0b0001000001010000,  // n
    0b0000000011011100,  // o
    0b0000000101110000,  // p
    0b0000010010000110,  // q
    0b0000000001010000,  // r
    0b0010000010001000,  // s
    0b0000000001111000,  // t
    0b0000000000011100,  // u
    0b0010000000000100,  // v
    0b0010100000010100,  // w
    0b0010100011000000,  // x
    0b0010000000001100,  // y
    0b0000100001001000,  // z
    0b0000100101001001,  // {
    0b0001001000000000,  // |
    0b0010010010001001,  // }
    0b0000000011100011   // ~ (126) displayed as '°' (encoded as ~) [was: 0b0000010100100000]
};
#endif

static const uint8_t numDigs[127-31-1+2] = {
    0b00000000, // space
    0b00000010, // !
    0b00100010, // "
    0b00110110, // #
    0b01101001, // $
    0b00101101, // %
    0b01111011, // &
    0b00100000, // '
    0b00111001, // (
    0b00001111, // )
    0b01100011, // *
    0b01110000, // +
    0b00001100, // ,
    0b01000000, // -
    0b00001000, // .
    0b01000010, // /
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111100, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01100111, // 9
    0b01001000, // :
    0b01001100, // ;
    0b01100001, // <
    0b01000001, // =
    0b01000011, // >
    0b01010011, // ? 
    0b01011111, // @
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001, // F
    0b00111101, // G
    0b01110110, // H
    0b00110000, // I
    0b00011110, // J
    0b01110101, // K
    0b00111000, // L
    0b00010101, // M
    0b00110111, // N
    0b00111111, // O
    0b01110011, // P
    0b01101011, // Q
    0b00110011, // R
    0b01101101, // S
    0b01111000, // T
    0b00111110, // U
    0b00111110, // V
    0b00101010, // W
    0b01110110, // X
    0b01101110, // Y
    0b01011011, // Z
    0b00111001, // [
    0b01100000, // backslash
    0b00001111, // ]
    0b00100011, // ^
    0b00001000, // _
    0b00100000, // ´
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001, // F
    0b00111101, // G
    0b01110110, // H
    0b00110000, // I
    0b00011110, // J
    0b01110101, // K
    0b00111000, // L
    0b00010101, // M
    0b00110111, // N
    0b00111111, // O
    0b01110011, // P
    0b01101011, // Q
    0b00110011, // R
    0b01101101, // S
    0b01111000, // T
    0b00111110, // U
    0b00111110, // V
    0b00101010, // W
    0b01110110, // X
    0b01101110, // Y
    0b01011011, // Z
    0b00111001, // {
    0b00110000, // |
    0b00001111, // }
    0b01100011, // ~  displayed as '°' (encoded as ~)
    0b01101011, // %1  (encoded as \x7f)
    0b01011101  // %2  (encoded as \x80)
};

#endif
