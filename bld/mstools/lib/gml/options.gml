:cmt *****************************************************************************
:cmt *
:cmt *                            Open Watcom Project
:cmt *
:cmt * Copyright (c) 2002-2020 The Open Watcom Contributors. All Rights Reserved.
:cmt *    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
:cmt *
:cmt *  ========================================================================
:cmt *
:cmt *    This file contains Original Code and/or Modifications of Original
:cmt *    Code as defined in and that are subject to the Sybase Open Watcom
:cmt *    Public License version 1.0 (the 'License'). You may not use this file
:cmt *    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
:cmt *    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
:cmt *    provided with the Original Code and Modifications, and is also
:cmt *    available at www.sybase.com/developer/opensource.
:cmt *
:cmt *    The Original Code and all software distributed under the License are
:cmt *    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
:cmt *    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
:cmt *    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
:cmt *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
:cmt *    NON-INFRINGEMENT. Please see the License for the specific language
:cmt *    governing rights and limitations under the License.
:cmt *
:cmt *  ========================================================================
:cmt *
:cmt * Description:  MSTOOLS lib command line options.
:cmt *
:cmt *     UTF-8 encoding, ¥
:cmt *
:cmt *****************************************************************************
:cmt

:cmt **********************************************************************
:cmt *       Copyright by WATCOM International Corporation, 1987, 1992.   *
:cmt *       All rights reserved. No part of this software may be         *
:cmt *       reproduced in any form or by any means - graphic, electronic,*
:cmt *       mechanical or otherwise, including, without limitation,      *
:cmt *       photocopying, recording, taping or information storage and   *
:cmt *       retrieval systems - except with the written permission of    *
:cmt *       WATCOM International Corporation.                            *
:cmt **********************************************************************
:cmt
:cmt    Modified    By              Reason
:cmt    --------    --              ------
:cmt    16-feb-96   A. Kucharczyk   Initial implementation.
:cmt    21-feb-96   A. Kucharczyk   Added /nowopts and /passwopts options
:cmt                                Changed /def option to supported
:cmt    27-feb-96   Greg Bentz      Added /nowwarn option.
:cmt    10-jun-96   M. Hildebrand   Added /nofuzzy option.
:cmt    10-jul-96   Greg Bentz      Added /noinvoke option.
:cmt
:cmt GML Macros used:
:cmt
:cmt    :chain. <char> <usage>                  options that start with <char>
:cmt                                            can be chained together e.g.,
:cmt                                            -oa -ox -ot => -oaxt
:cmt    :option. <option> <synonym> ...         define an option
:cmt    :target. <arch1> <arch2> ...            valid for these architectures
:cmt    :ntarget. <arch1> <arch2> ...           not valid for these architectures
:cmt    :immediate. <fn>                        <fn> is called when option parsed
:cmt    :enumerate. <field> [<value>]           option is one value in <name> enumeration
:cmt    :number. [<fn>] [<default>]             =<n> allowed; call <fn> to check
:cmt    :id. [<fn>]                             =<id> req'd; call <fn> to check
:cmt    :char.[<fn>]                            =<char> req'd; call <fn> to check
:cmt    :file.                                  =<file> req'd
:cmt    :path.                                  =<path> req'd
:cmt    :special. <fn>                          call <fn> to parse option
:cmt    :optional.                              value is optional
:cmt    :internal.                              option is undocumented
:cmt    :prefix.                                prefix of a :special. option
:cmt    :usageogrp. <option> <usage text>       group of options that start with <option>
:cmt                                            they are chained together in usage text only
:cmt    :usage. <text>                          English usage text
:cmt    :jusage. <text>                         Japanese usage text
:cmt    :title.                                 English usage text
:cmt    :timestamp.                             kludge to record "when" an option
:cmt                                            is set so that dependencies
:cmt                                            between options can be simulated
:cmt
:cmt Global macros
:cmt
:cmt    :noequal.                               args can't have option '='
:cmt    :argequal. <char>                       args use <char> instead of '='
:cmt

:cmt    where:
:cmt        <arch>:     i86, 386, axp, any, dbg, qnx

:page. \t(Press return to continue)

:title. Usage: lib [options] [files] [@commandfile]
:target. any
:title. Options:
:target. any
:title.  .         ( /option is also accepted )
:target. any
:ntarget. qnx linux osx bsd haiku
                                                                         

:argequal. :


:cmt    /DEBUGTYPE:{COFF|CV|BOTH}
:option. debugtype
:target. any
:special. parse_debugtype
:internal.
:usage. include specified type of debugging information
    
:cmt    /DEF:[filename]
:option. def
:target. any
:special. parse_def :<file>
:usage. process .def file

:cmt    /EXPORT:symbol
:option. export
:target. any
:special. parse_export :<symbol>
:usage. export a symbol

:cmt    /EXTRACT:membername
:option. extract
:target. any
:special. parse_extract :<membername>
:usage. extract member from library

:cmt    /IMPORT:[CURRENTVER=#][,][OLDCODEVER=#][,][OLDAPIVER=#]
:option. import
:target. any
:special. parse_import :<symbol>
:internal.
:usage. import a symbol


:cmt    /INCLUDE:symbol
:option. include
:target. any
:special. parse_include :<symbol>
:internal.
:usage. include a symbol

:cmt    /LIST[:filename]
:option. list
:target. any
:special. parse_list [:<file>]
:usage. generate list file

:cmt    /MAC:{INIT=symbol|TERM=symbol}
:option. mac
:target. any
:special. parse_mac
:internal.
:usage. specify Macintosh platform options

:cmt    /MACHINE:{IX86|MIPS|ALPHA|PPC|M68K|MPPC}
:option. machine
:target. any
:special. parse_machine :<IX86|ALPHA>
:usage. specify target platform

:cmt    /NAME:filename
:option. name
:target. any
:special. parse_name :<file>
:internal.
:usage. specify output file name

:cmt    /NODEFAULTLIB[:library]
:option. nodefaultlib
:target. any
:special. parse_nodefaultlib [:library]
:internal.
:usage. ignore all default libraries

:option. nofuzzy
:target. any
:usage. disable fuzzy linking

:option. noinvoke
:target. any
:usage. don't invoke the Watcom tool

:cmt    /NOLOGO
:option. nologo
:target. any
:usage. don't display copyright banner

:option. nowopts
:target. any
:usage. disable default options

:option. nowwarn
:target. any
:immediate. handle_nowwarn
:usage. disable warning messages for ignored options

:cmt    /OUT:filename
:option. out
:target. any
:special. parse_out :<file>
:usage. specify output file name

:option. passwopts
:target. any
:special. parse_passwopts :<options>
:usage. pass <options> directly to the Watcom tools

:cmt    /REMOVE:membername
:option. remove
:target. any
:special. parse_remove :<membername>
:usage. remove member from library

:cmt    /SUBSYSTEM:{NATIVE|WINDOWS|CONSOLE|POSIX}[,#[.#]]
:option. subsystem
:target. any
:special. parse_subsystem
:internal.
:usage. tell OS how to run the executable

:option. showwopts
:target. any
:usage. show translated options

:cmt    /VERBOSE
:option. verbose
:target. any
:internal.
:usage. enable verbose messages

