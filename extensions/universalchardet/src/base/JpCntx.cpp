/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nscore.h"
#include "JpCntx.h"

//This is hiragana 2-char sequence table, the number in each cell represents its frequency category
const uint8_t jp2CharContext[83][83] =
{
{ 0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,},
{ 2,4,0,4,0,3,0,4,0,3,4,4,4,2,4,3,3,4,3,2,3,3,4,2,3,3,3,2,4,1,4,3,3,1,5,4,3,4,3,4,3,5,3,0,3,5,4,2,0,3,1,0,3,3,0,3,3,0,1,1,0,4,3,0,3,3,0,4,0,2,0,3,5,5,5,5,4,0,4,1,0,3,4,},
{ 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,},
{ 0,4,0,5,0,5,0,4,0,4,5,4,4,3,5,3,5,1,5,3,4,3,4,4,3,4,3,3,4,3,5,4,4,3,5,5,3,5,5,5,3,5,5,3,4,5,5,3,1,3,2,0,3,4,0,4,2,0,4,2,1,5,3,2,3,5,0,4,0,2,0,5,4,4,5,4,5,0,4,0,0,4,4,},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{ 0,3,0,4,0,3,0,3,0,4,5,4,3,3,3,3,4,3,5,4,4,3,5,4,4,3,4,3,4,4,4,4,5,3,4,4,3,4,5,5,4,5,5,1,4,5,4,3,0,3,3,1,3,3,0,4,4,0,3,3,1,5,3,3,3,5,0,4,0,3,0,4,4,3,4,3,3,0,4,1,1,3,4,},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{ 0,4,0,3,0,3,0,4,0,3,4,4,3,2,2,1,2,1,3,1,3,3,3,3,3,4,3,1,3,3,5,3,3,0,4,3,0,5,4,3,3,5,4,4,3,4,4,5,0,1,2,0,1,2,0,2,2,0,1,0,0,5,2,2,1,4,0,3,0,1,0,4,4,3,5,4,3,0,2,1,0,4,3,},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{ 0,3,0,5,0,4,0,2,1,4,4,2,4,1,4,2,4,2,4,3,3,3,4,3,3,3,3,1,4,2,3,3,3,1,4,4,1,1,1,4,3,3,2,0,2,4,3,2,0,3,3,0,3,1,1,0,0,0,3,3,0,4,2,2,3,4,0,4,0,3,0,4,4,5,3,4,4,0,3,0,0,1,4,},
{ 1,4,0,4,0,4,0,4,0,3,5,4,4,3,4,3,5,4,3,3,4,3,5,4,4,4,4,3,4,2,4,3,3,1,5,4,3,2,4,5,4,5,5,4,4,5,4,4,0,3,2,2,3,3,0,4,3,1,3,2,1,4,3,3,4,5,0,3,0,2,0,4,5,5,4,5,4,0,4,0,0,5,4,},
{ 0,5,0,5,0,4,0,3,0,4,4,3,4,3,3,3,4,0,4,4,4,3,4,3,4,3,3,1,4,2,4,3,4,0,5,4,1,4,5,4,4,5,3,2,4,3,4,3,2,4,1,3,3,3,2,3,2,0,4,3,3,4,3,3,3,4,0,4,0,3,0,4,5,4,4,4,3,0,4,1,0,1,3,},
{ 0,3,1,4,0,3,0,2,0,3,4,4,3,1,4,2,3,3,4,3,4,3,4,3,4,4,3,2,3,1,5,4,4,1,4,4,3,5,4,4,3,5,5,4,3,4,4,3,1,2,3,1,2,2,0,3,2,0,3,1,0,5,3,3,3,4,3,3,3,3,4,4,4,4,5,4,2,0,3,3,2,4,3,},
{ 0,2,0,3,0,1,0,1,0,0,3,2,0,0,2,0,1,0,2,1,3,3,3,1,2,3,1,0,1,0,4,2,1,1,3,3,0,4,3,3,1,4,3,3,0,3,3,2,0,0,0,0,1,0,0,2,0,0,0,0,0,4,1,0,2,3,2,2,2,1,3,3,3,4,4,3,2,0,3,1,0,3,3,},
{ 0,4,0,4,0,3,0,3,0,4,4,4,3,3,3,3,3,3,4,3,4,2,4,3,4,3,3,2,4,3,4,5,4,1,4,5,3,5,4,5,3,5,4,0,3,5,5,3,1,3,3,2,2,3,0,3,4,1,3,3,2,4,3,3,3,4,0,4,0,3,0,4,5,4,4,5,3,0,4,1,0,3,4,},
{ 0,2,0,3,0,3,0,0,0,2,2,2,1,0,1,0,0,0,3,0,3,0,3,0,1,3,1,0,3,1,3,3,3,1,3,3,3,0,1,3,1,3,4,0,0,3,1,1,0,3,2,0,0,0,0,1,3,0,1,0,0,3,3,2,0,3,0,0,0,0,0,3,4,3,4,3,3,0,3,0,0,2,3,},
{ 2,3,0,3,0,2,0,1,0,3,3,4,3,1,3,1,1,1,3,1,4,3,4,3,3,3,0,0,3,1,5,4,3,1,4,3,2,5,5,4,4,4,4,3,3,4,4,4,0,2,1,1,3,2,0,1,2,0,0,1,0,4,1,3,3,3,0,3,0,1,0,4,4,4,5,5,3,0,2,0,0,4,4,},
{ 0,2,0,1,0,3,1,3,0,2,3,3,3,0,3,1,0,0,3,0,3,2,3,1,3,2,1,1,0,0,4,2,1,0,2,3,1,4,3,2,0,4,4,3,1,3,1,3,0,1,0,0,1,0,0,0,1,0,0,0,0,4,1,1,1,2,0,3,0,0,0,3,4,2,4,3,2,0,1,0,0,3,3,},
{ 0,1,0,4,0,5,0,4,0,2,4,4,2,3,3,2,3,3,5,3,3,3,4,3,4,2,3,0,4,3,3,3,4,1,4,3,2,1,5,5,3,4,5,1,3,5,4,2,0,3,3,0,1,3,0,4,2,0,1,3,1,4,3,3,3,3,0,3,0,1,0,3,4,4,4,5,5,0,3,0,1,4,5,},
{ 0,2,0,3,0,3,0,0,0,2,3,1,3,0,4,0,1,1,3,0,3,4,3,2,3,1,0,3,3,2,3,1,3,0,2,3,0,2,1,4,1,2,2,0,0,3,3,0,0,2,0,0,0,1,0,0,0,0,2,2,0,3,2,1,3,3,0,2,0,2,0,0,3,3,1,2,4,0,3,0,2,2,3,},
{ 2,4,0,5,0,4,0,4,0,2,4,4,4,3,4,3,3,3,1,2,4,3,4,3,4,4,5,0,3,3,3,3,2,0,4,3,1,4,3,4,1,4,4,3,3,4,4,3,1,2,3,0,4,2,0,4,1,0,3,3,0,4,3,3,3,4,0,4,0,2,0,3,5,3,4,5,2,0,3,0,0,4,5,},
{ 0,3,0,4,0,1,0,1,0,1,3,2,2,1,3,0,3,0,2,0,2,0,3,0,2,0,0,0,1,0,1,1,0,0,3,1,0,0,0,4,0,3,1,0,2,1,3,0,0,0,0,0,0,3,0,0,0,0,0,0,0,4,2,2,3,1,0,3,0,0,0,1,4,4,4,3,0,0,4,0,0,1,4,},
{ 1,4,1,5,0,3,0,3,0,4,5,4,4,3,5,3,3,4,4,3,4,1,3,3,3,3,2,1,4,1,5,4,3,1,4,4,3,5,4,4,3,5,4,3,3,4,4,4,0,3,3,1,2,3,0,3,1,0,3,3,0,5,4,4,4,4,4,4,3,3,5,4,4,3,3,5,4,0,3,2,0,4,4,},
{ 0,2,0,3,0,1,0,0,0,1,3,3,3,2,4,1,3,0,3,1,3,0,2,2,1,1,0,0,2,0,4,3,1,0,4,3,0,4,4,4,1,4,3,1,1,3,3,1,0,2,0,0,1,3,0,0,0,0,2,0,0,4,3,2,4,3,5,4,3,3,3,4,3,3,4,3,3,0,2,1,0,3,3,},
{ 0,2,0,4,0,3,0,2,0,2,5,5,3,4,4,4,4,1,4,3,3,0,4,3,4,3,1,3,3,2,4,3,0,3,4,3,0,3,4,4,2,4,4,0,4,5,3,3,2,2,1,1,1,2,0,1,5,0,3,3,2,4,3,3,3,4,0,3,0,2,0,4,4,3,5,5,0,0,3,0,2,3,3,},
{ 0,3,0,4,0,3,0,1,0,3,4,3,3,1,3,3,3,0,3,1,3,0,4,3,3,1,1,0,3,0,3,3,0,0,4,4,0,1,5,4,3,3,5,0,3,3,4,3,0,2,0,1,1,1,0,1,3,0,1,2,1,3,3,2,3,3,0,3,0,1,0,1,3,3,4,4,1,0,1,2,2,1,3,},
{ 0,1,0,4,0,4,0,3,0,1,3,3,3,2,3,1,1,0,3,0,3,3,4,3,2,4,2,0,1,0,4,3,2,0,4,3,0,5,3,3,2,4,4,4,3,3,3,4,0,1,3,0,0,1,0,0,1,0,0,0,0,4,2,3,3,3,0,3,0,0,0,4,4,4,5,3,2,0,3,3,0,3,5,},
{ 0,2,0,3,0,0,0,3,0,1,3,0,2,0,0,0,1,0,3,1,1,3,3,0,0,3,0,0,3,0,2,3,1,0,3,1,0,3,3,2,0,4,2,2,0,2,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,2,1,2,0,1,0,1,0,0,0,1,3,1,2,0,0,0,1,0,0,1,4,},
{ 0,3,0,3,0,5,0,1,0,2,4,3,1,3,3,2,1,1,5,2,1,0,5,1,2,0,0,0,3,3,2,2,3,2,4,3,0,0,3,3,1,3,3,0,2,5,3,4,0,3,3,0,1,2,0,2,2,0,3,2,0,2,2,3,3,3,0,2,0,1,0,3,4,4,2,5,4,0,3,0,0,3,5,},
{ 0,3,0,3,0,3,0,1,0,3,3,3,3,0,3,0,2,0,2,1,1,0,2,0,1,0,0,0,2,1,0,0,1,0,3,2,0,0,3,3,1,2,3,1,0,3,3,0,0,1,0,0,0,0,0,2,0,0,0,0,0,2,3,1,2,3,0,3,0,1,0,3,2,1,0,4,3,0,1,1,0,3,3,},
{ 0,4,0,5,0,3,0,3,0,4,5,5,4,3,5,3,4,3,5,3,3,2,5,3,4,4,4,3,4,3,4,5,5,3,4,4,3,4,4,5,4,4,4,3,4,5,5,4,2,3,4,2,3,4,0,3,3,1,4,3,2,4,3,3,5,5,0,3,0,3,0,5,5,5,5,4,4,0,4,0,1,4,4,},
{ 0,4,0,4,0,3,0,3,0,3,5,4,4,2,3,2,5,1,3,2,5,1,4,2,3,2,3,3,4,3,3,3,3,2,5,4,1,3,3,5,3,4,4,0,4,4,3,1,1,3,1,0,2,3,0,2,3,0,3,0,0,4,3,1,3,4,0,3,0,2,0,4,4,4,3,4,5,0,4,0,0,3,4,},
{ 0,3,0,3,0,3,1,2,0,3,4,4,3,3,3,0,2,2,4,3,3,1,3,3,3,1,1,0,3,1,4,3,2,3,4,4,2,4,4,4,3,4,4,3,2,4,4,3,1,3,3,1,3,3,0,4,1,0,2,2,1,4,3,2,3,3,5,4,3,3,5,4,4,3,3,0,4,0,3,2,2,4,4,},
{ 0,2,0,1,0,0,0,0,0,1,2,1,3,0,0,0,0,0,2,0,1,2,1,0,0,1,0,0,0,0,3,0,0,1,0,1,1,3,1,0,0,0,1,1,0,1,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,1,2,2,0,3,4,0,0,0,1,1,0,0,1,0,0,0,0,0,1,1,},
{ 0,1,0,0,0,1,0,0,0,0,4,0,4,1,4,0,3,0,4,0,3,0,4,0,3,0,3,0,4,1,5,1,4,0,0,3,0,5,0,5,2,0,1,0,0,0,2,1,4,0,1,3,0,0,3,0,0,3,1,1,4,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,},
{ 1,4,0,5,0,3,0,2,0,3,5,4,4,3,4,3,5,3,4,3,3,0,4,3,3,3,3,3,3,2,4,4,3,1,3,4,4,5,4,4,3,4,4,1,3,5,4,3,3,3,1,2,2,3,3,1,3,1,3,3,3,5,3,3,4,5,0,3,0,3,0,3,4,3,4,4,3,0,3,0,2,4,3,},
{ 0,1,0,4,0,0,0,0,0,1,4,0,4,1,4,2,4,0,3,0,1,0,1,0,0,0,0,0,2,0,3,1,1,1,0,3,0,0,0,1,2,1,0,0,1,1,1,1,0,1,0,0,0,1,0,0,3,0,0,0,0,3,2,0,2,2,0,1,0,0,0,2,3,2,3,3,0,0,0,0,2,1,0,},
{ 0,5,1,5,0,3,0,3,0,5,4,4,5,1,5,3,3,0,4,3,4,3,5,3,4,3,3,2,4,3,4,3,3,0,3,3,1,4,4,3,4,4,4,3,4,5,5,3,2,3,1,1,3,3,1,3,1,1,3,3,2,4,5,3,3,5,0,4,0,3,0,4,4,3,5,3,3,0,3,4,0,4,3,},
{ 0,5,0,5,0,3,0,2,0,4,4,3,5,2,4,3,3,3,4,4,4,3,5,3,5,3,3,1,4,0,4,3,3,0,3,3,0,4,4,4,4,5,4,3,3,5,5,3,2,3,1,2,3,2,0,1,0,0,3,2,2,4,4,3,1,5,0,4,0,3,0,4,3,1,3,2,1,0,3,3,0,3,3,},
{ 0,4,0,5,0,5,0,4,0,4,5,5,5,3,4,3,3,2,5,4,4,3,5,3,5,3,4,0,4,3,4,4,3,2,4,4,3,4,5,4,4,5,5,0,3,5,5,4,1,3,3,2,3,3,1,3,1,0,4,3,1,4,4,3,4,5,0,4,0,2,0,4,3,4,4,3,3,0,4,0,0,5,5,},
{ 0,4,0,4,0,5,0,1,1,3,3,4,4,3,4,1,3,0,5,1,3,0,3,1,3,1,1,0,3,0,3,3,4,0,4,3,0,4,4,4,3,4,4,0,3,5,4,1,0,3,0,0,2,3,0,3,1,0,3,1,0,3,2,1,3,5,0,3,0,1,0,3,2,3,3,4,4,0,2,2,0,4,4,},
{ 2,4,0,5,0,4,0,3,0,4,5,5,4,3,5,3,5,3,5,3,5,2,5,3,4,3,3,4,3,4,5,3,2,1,5,4,3,2,3,4,5,3,4,1,2,5,4,3,0,3,3,0,3,2,0,2,3,0,4,1,0,3,4,3,3,5,0,3,0,1,0,4,5,5,5,4,3,0,4,2,0,3,5,},
{ 0,5,0,4,0,4,0,2,0,5,4,3,4,3,4,3,3,3,4,3,4,2,5,3,5,3,4,1,4,3,4,4,4,0,3,5,0,4,4,4,4,5,3,1,3,4,5,3,3,3,3,3,3,3,0,2,2,0,3,3,2,4,3,3,3,5,3,4,1,3,3,5,3,2,0,0,0,0,4,3,1,3,3,},
{ 0,1,0,3,0,3,0,1,0,1,3,3,3,2,3,3,3,0,3,0,0,0,3,1,3,0,0,0,2,2,2,3,0,0,3,2,0,1,2,4,1,3,3,0,0,3,3,3,0,1,0,0,2,1,0,0,3,0,3,1,0,3,0,0,1,3,0,2,0,1,0,3,3,1,3,3,0,0,1,1,0,3,3,},
{ 0,2,0,3,0,2,1,4,0,2,2,3,1,1,3,1,1,0,2,0,3,1,2,3,1,3,0,0,1,0,4,3,2,3,3,3,1,4,2,3,3,3,3,1,0,3,1,4,0,1,1,0,1,2,0,1,1,0,1,1,0,3,1,3,2,2,0,1,0,0,0,2,3,3,3,1,0,0,0,0,0,2,3,},
{ 0,5,0,4,0,5,0,2,0,4,5,5,3,3,4,3,3,1,5,4,4,2,4,4,4,3,4,2,4,3,5,5,4,3,3,4,3,3,5,5,4,5,5,1,3,4,5,3,1,4,3,1,3,3,0,3,3,1,4,3,1,4,5,3,3,5,0,4,0,3,0,5,3,3,1,4,3,0,4,0,1,5,3,},
{ 0,5,0,5,0,4,0,2,0,4,4,3,4,3,3,3,3,3,5,4,4,4,4,4,4,5,3,3,5,2,4,4,4,3,4,4,3,3,4,4,5,5,3,3,4,3,4,3,3,4,3,3,3,3,1,2,2,1,4,3,3,5,4,4,3,4,0,4,0,3,0,4,4,4,4,4,1,0,4,2,0,2,4,},
{ 0,4,0,4,0,3,0,1,0,3,5,2,3,0,3,0,2,1,4,2,3,3,4,1,4,3,3,2,4,1,3,3,3,0,3,3,0,0,3,3,3,5,3,3,3,3,3,2,0,2,0,0,2,0,0,2,0,0,1,0,0,3,1,2,2,3,0,3,0,2,0,4,4,3,3,4,1,0,3,0,0,2,4,},
{ 0,0,0,4,0,0,0,0,0,0,1,0,1,0,2,0,0,0,0,0,1,0,2,0,1,0,0,0,0,0,3,1,3,0,3,2,0,0,0,1,0,3,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,0,2,0,0,0,0,0,0,2,},
{ 0,2,1,3,0,2,0,2,0,3,3,3,3,1,3,1,3,3,3,3,3,3,4,2,2,1,2,1,4,0,4,3,1,3,3,3,2,4,3,5,4,3,3,3,3,3,3,3,0,1,3,0,2,0,0,1,0,0,1,0,0,4,2,0,2,3,0,3,3,0,3,3,4,2,3,1,4,0,1,2,0,2,3,},
{ 0,3,0,3,0,1,0,3,0,2,3,3,3,0,3,1,2,0,3,3,2,3,3,2,3,2,3,1,3,0,4,3,2,0,3,3,1,4,3,3,2,3,4,3,1,3,3,1,1,0,1,1,0,1,0,1,0,1,0,0,0,4,1,1,0,3,0,3,1,0,2,3,3,3,3,3,1,0,0,2,0,3,3,},
{ 0,0,0,0,0,0,0,0,0,0,3,0,2,0,3,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,0,3,0,3,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,2,0,2,3,0,0,0,0,0,0,0,0,3,},
{ 0,2,0,3,1,3,0,3,0,2,3,3,3,1,3,1,3,1,3,1,3,3,3,1,3,0,2,3,1,1,4,3,3,2,3,3,1,2,2,4,1,3,3,0,1,4,2,3,0,1,3,0,3,0,0,1,3,0,2,0,0,3,3,2,1,3,0,3,0,2,0,3,4,4,4,3,1,0,3,0,0,3,3,},
{ 0,2,0,1,0,2,0,0,0,1,3,2,2,1,3,0,1,1,3,0,3,2,3,1,2,0,2,0,1,1,3,3,3,0,3,3,1,1,2,3,2,3,3,1,2,3,2,0,0,1,0,0,0,0,0,0,3,0,1,0,0,2,1,2,1,3,0,3,0,0,0,3,4,4,4,3,2,0,2,0,0,2,4,},
{ 0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,2,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,3,1,0,0,0,0,0,0,0,3,},
{ 0,3,0,3,0,2,0,3,0,3,3,3,2,3,2,2,2,0,3,1,3,3,3,2,3,3,0,0,3,0,3,2,2,0,2,3,1,4,3,4,3,3,2,3,1,5,4,4,0,3,1,2,1,3,0,3,1,1,2,0,2,3,1,3,1,3,0,3,0,1,0,3,3,4,4,2,1,0,2,1,0,2,4,},
{ 0,1,0,3,0,1,0,2,0,1,4,2,5,1,4,0,2,0,2,1,3,1,4,0,2,1,0,0,2,1,4,1,1,0,3,3,0,5,1,3,2,3,3,1,0,3,2,3,0,1,0,0,0,0,0,0,1,0,0,0,0,4,0,1,0,3,0,2,0,1,0,3,3,3,4,3,3,0,0,0,0,2,3,},
{ 0,0,0,1,0,0,0,0,0,0,2,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,1,0,0,0,0,0,3,},
{ 0,1,0,3,0,4,0,3,0,2,4,3,1,0,3,2,2,1,3,1,2,2,3,1,1,1,2,1,3,0,1,2,0,1,3,2,1,3,0,5,5,1,0,0,1,3,2,1,0,3,0,0,1,0,0,0,0,0,3,4,0,1,1,1,3,2,0,2,0,1,0,2,3,3,1,2,3,0,1,0,1,0,4,},
{ 0,0,0,1,0,3,0,3,0,2,2,1,0,0,4,0,3,0,3,1,3,0,3,0,3,0,1,0,3,0,3,1,3,0,3,3,0,0,1,2,1,1,1,0,1,2,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,2,1,2,0,0,2,0,0,0,0,2,3,3,3,3,0,0,0,0,1,4,},
{ 0,0,0,3,0,3,0,0,0,0,3,1,1,0,3,0,1,0,2,0,1,0,0,0,0,0,0,0,1,0,3,0,2,0,2,3,0,0,2,2,3,1,2,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,2,0,0,0,0,2,3,},
{ 2,4,0,5,0,5,0,4,0,3,4,3,3,3,4,3,3,3,4,3,4,4,5,4,5,5,5,2,3,0,5,5,4,1,5,4,3,1,5,4,3,4,4,3,3,4,3,3,0,3,2,0,2,3,0,3,0,0,3,3,0,5,3,2,3,3,0,3,0,3,0,3,4,5,4,5,3,0,4,3,0,3,4,},
{ 0,3,0,3,0,3,0,3,0,3,3,4,3,2,3,2,3,0,4,3,3,3,3,3,3,3,3,0,3,2,4,3,3,1,3,4,3,4,4,4,3,4,4,3,2,4,4,1,0,2,0,0,1,1,0,2,0,0,3,1,0,5,3,2,1,3,0,3,0,1,2,4,3,2,4,3,3,0,3,2,0,4,4,},
{ 0,3,0,3,0,1,0,0,0,1,4,3,3,2,3,1,3,1,4,2,3,2,4,2,3,4,3,0,2,2,3,3,3,0,3,3,3,0,3,4,1,3,3,0,3,4,3,3,0,1,1,0,1,0,0,0,4,0,3,0,0,3,1,2,1,3,0,4,0,1,0,4,3,3,4,3,3,0,2,0,0,3,3,},
{ 0,3,0,4,0,1,0,3,0,3,4,3,3,0,3,3,3,1,3,1,3,3,4,3,3,3,0,0,3,1,5,3,3,1,3,3,2,5,4,3,3,4,5,3,2,5,3,4,0,1,0,0,0,0,0,2,0,0,1,1,0,4,2,2,1,3,0,3,0,2,0,4,4,3,5,3,2,0,1,1,0,3,4,},
{ 0,5,0,4,0,5,0,2,0,4,4,3,3,2,3,3,3,1,4,3,4,1,5,3,4,3,4,0,4,2,4,3,4,1,5,4,0,4,4,4,4,5,4,1,3,5,4,2,1,4,1,1,3,2,0,3,1,0,3,2,1,4,3,3,3,4,0,4,0,3,0,4,4,4,3,3,3,0,4,2,0,3,4,},
{ 1,4,0,4,0,3,0,1,0,3,3,3,1,1,3,3,2,2,3,3,1,0,3,2,2,1,2,0,3,1,2,1,2,0,3,2,0,2,2,3,3,4,3,0,3,3,1,2,0,1,1,3,1,2,0,0,3,0,1,1,0,3,2,2,3,3,0,3,0,0,0,2,3,3,4,3,3,0,1,0,0,1,4,},
{ 0,4,0,4,0,4,0,0,0,3,4,4,3,1,4,2,3,2,3,3,3,1,4,3,4,0,3,0,4,2,3,3,2,2,5,4,2,1,3,4,3,4,3,1,3,3,4,2,0,2,1,0,3,3,0,0,2,0,3,1,0,4,4,3,4,3,0,4,0,1,0,2,4,4,4,4,4,0,3,2,0,3,3,},
{ 0,0,0,1,0,4,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,3,2,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,2,},
{ 0,2,0,3,0,4,0,4,0,1,3,3,3,0,4,0,2,1,2,1,1,1,2,0,3,1,1,0,1,0,3,1,0,0,3,3,2,0,1,1,0,0,0,0,0,1,0,2,0,2,2,0,3,1,0,0,1,0,1,1,0,1,2,0,3,0,0,0,0,1,0,0,3,3,4,3,1,0,1,0,3,0,2,},
{ 0,0,0,3,0,5,0,0,0,0,1,0,2,0,3,1,0,1,3,0,0,0,2,0,0,0,1,0,0,0,1,1,0,0,4,0,0,0,2,3,0,1,4,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,0,0,0,0,0,0,0,2,0,0,3,0,0,0,0,0,3,},
{ 0,2,0,5,0,5,0,1,0,2,4,3,3,2,5,1,3,2,3,3,3,0,4,1,2,0,3,0,4,0,2,2,1,1,5,3,0,0,1,4,2,3,2,0,3,3,3,2,0,2,4,1,1,2,0,1,1,0,3,1,0,1,3,1,2,3,0,2,0,0,0,1,3,5,4,4,4,0,3,0,0,1,3,},
{ 0,4,0,5,0,4,0,4,0,4,5,4,3,3,4,3,3,3,4,3,4,4,5,3,4,5,4,2,4,2,3,4,3,1,4,4,1,3,5,4,4,5,5,4,4,5,5,5,2,3,3,1,4,3,1,3,3,0,3,3,1,4,3,4,4,4,0,3,0,4,0,3,3,4,4,5,0,0,4,3,0,4,5,},
{ 0,4,0,4,0,3,0,3,0,3,4,4,4,3,3,2,4,3,4,3,4,3,5,3,4,3,2,1,4,2,4,4,3,1,3,4,2,4,5,5,3,4,5,4,1,5,4,3,0,3,2,2,3,2,1,3,1,0,3,3,3,5,3,3,3,5,4,4,2,3,3,4,3,3,3,2,1,0,3,2,1,4,3,},
{ 0,4,0,5,0,4,0,3,0,3,5,5,3,2,4,3,4,0,5,4,4,1,4,4,4,3,3,3,4,3,5,5,2,3,3,4,1,2,5,5,3,5,5,2,3,5,5,4,0,3,2,0,3,3,1,1,5,1,4,1,0,4,3,2,3,5,0,4,0,3,0,5,4,3,4,3,0,0,4,1,0,4,4,},
{ 1,3,0,4,0,2,0,2,0,2,5,5,3,3,3,3,3,0,4,2,3,4,4,4,3,4,0,0,3,4,5,4,3,3,3,3,2,5,5,4,5,5,5,4,3,5,5,5,1,3,1,0,1,0,0,3,2,0,4,2,0,5,2,3,2,4,1,3,0,3,0,4,5,4,5,4,3,0,4,2,0,5,4,},
{ 0,3,0,4,0,5,0,3,0,3,4,4,3,2,3,2,3,3,3,3,3,2,4,3,3,2,2,0,3,3,3,3,3,1,3,3,3,0,4,4,3,4,4,1,1,4,4,2,0,3,1,0,1,1,0,4,1,0,2,3,1,3,3,1,3,4,0,3,0,1,0,3,1,3,0,0,1,0,2,0,0,4,4,},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{ 0,3,0,3,0,2,0,3,0,1,5,4,3,3,3,1,4,2,1,2,3,4,4,2,4,4,5,0,3,1,4,3,4,0,4,3,3,3,2,3,2,5,3,4,3,2,2,3,0,0,3,0,2,1,0,1,2,0,0,0,0,2,1,1,3,1,0,2,0,4,0,3,4,4,4,5,2,0,2,0,0,1,3,},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,4,2,1,1,0,1,0,3,2,0,0,3,1,1,1,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,0,0,0,2,0,0,0,1,4,0,4,2,1,0,0,0,0,0,1,},
{ 0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,3,1,0,0,0,2,0,2,1,0,0,1,2,1,0,1,1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,1,3,1,0,0,0,0,0,1,0,0,2,1,0,0,0,0,0,0,0,0,2,},
{ 0,4,0,4,0,4,0,3,0,4,4,3,4,2,4,3,2,0,4,4,4,3,5,3,5,3,3,2,4,2,4,3,4,3,1,4,0,2,3,4,4,4,3,3,3,4,4,4,3,4,1,3,4,3,2,1,2,1,3,3,3,4,4,3,3,5,0,4,0,3,0,4,3,3,3,2,1,0,3,0,0,3,3,},
{ 0,4,0,3,0,3,0,3,0,3,5,5,3,3,3,3,4,3,4,3,3,3,4,4,4,3,3,3,3,4,3,5,3,3,1,3,2,4,5,5,5,5,4,3,4,5,5,3,2,2,3,3,3,3,2,3,3,1,2,3,2,4,3,3,3,4,0,4,0,2,0,4,3,2,2,1,2,0,3,0,0,4,1,},
};

#define MINIMUM_DATA_THRESHOLD  4

void JapaneseContextAnalysis::HandleData(const char* aBuf, uint32_t aLen)
{
  uint32_t charLen;
  int32_t order;
  uint32_t i;

  if (mDone)
    return;

  //The buffer we got is byte oriented, and a character may span in more than one
  //buffers. In case the last one or two byte in last buffer is not complete, we
  //record how many byte needed to complete that character and skip these bytes here.
  //We can choose to record those bytes as well and analyse the character once it
  //is complete, but since a character will not make much difference, by simply skipping
  //this character will simply our logic and improve performance.
  for (i = mNeedToSkipCharNum; i < aLen; )
  {
    order = GetOrder(aBuf+i, &charLen);
    i+= charLen;
    if (i > aLen){
      mNeedToSkipCharNum = i - aLen;
      mLastCharOrder = -1;
    }
    else
    {
      if (order != -1 && mLastCharOrder != -1)
      {
        mTotalRel ++;
        if (mTotalRel > MAX_REL_THRESHOLD)
        {
          mDone = true;
          break;
        }
        mRelSample[jp2CharContext[mLastCharOrder][order]]++;
      }
      mLastCharOrder = order;
    }
  }

  return;
}

void JapaneseContextAnalysis::Reset()
{
  mTotalRel = 0;
  for (uint32_t i = 0; i < NUM_OF_CATEGORY; i++)
    mRelSample[i] = 0;
  mNeedToSkipCharNum = 0;
  mLastCharOrder = -1;
  mDone = false;
  mDataThreshold = 0;
}
#define DONT_KNOW (float)-1

float  JapaneseContextAnalysis::GetConfidence(void)
{
  //This is just one way to calculate confidence. It works well for me.
  if (mTotalRel > mDataThreshold)
    return ((float)(mTotalRel - mRelSample[0]))/mTotalRel;
  else
    return (float)DONT_KNOW;
}


int32_t SJISContextAnalysis::GetOrder(const char* str, uint32_t *charLen)
{
  //find out current char's byte length
  if (((unsigned char)*str >= (unsigned char)0x81 && (unsigned char)*str <= (unsigned char)0x9f) ||
      ((unsigned char)*str >= (unsigned char)0xe0 && (unsigned char)*str <= (unsigned char)0xfc) )
      *charLen = 2;
  else
      *charLen = 1;

  //return its order if it is hiragana
  if (*str == '\202' &&
        (unsigned char)*(str+1) >= (unsigned char)0x9f &&
        (unsigned char)*(str+1) <= (unsigned char)0xf1)
    return (unsigned char)*(str+1) - (unsigned char)0x9f;
  return -1;
}

int32_t EUCJPContextAnalysis::GetOrder(const char* str, uint32_t *charLen)
{
  //find out current char's byte length
  if ((unsigned char)*str == (unsigned char)0x8e ||
      ((unsigned char)*str >= (unsigned char)0xa1 &&
       (unsigned char)*str <= (unsigned char)0xfe))
      *charLen = 2;
  else if ((unsigned char)*str == (unsigned char)0x8f)
    *charLen = 3;
  else
    *charLen = 1;

  //return its order if it is hiragana
  if ((unsigned char)*str == (unsigned char)0xa4 &&
      (unsigned char)*(str+1) >= (unsigned char)0xa1 &&
      (unsigned char)*(str+1) <= (unsigned char)0xf3)
     return (unsigned char)*(str+1) - (unsigned char)0xa1;
  return -1;
}
