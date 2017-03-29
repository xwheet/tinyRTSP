# Ragel generator
# For more information about Ragel: rtsp://www.complang.org/ragel/

#export OPTIONS="-C -L -G2"
export OPTIONS="-C -L -T0"

# RTSP URL parser
ragel.exe $OPTIONS -o ./src/parsers/trtsp_parser_url.c ./ragel/trtsp_parser_url.rl

# HTTP message (both requests an responses) parser.
ragel.exe $OPTIONS -o ./src/parsers/trtsp_parser_message.c ./ragel/trtsp_parser_message.rl

# HTTP headers parser
ragel.exe -C -L -T0 -o ./src/parsers/trtsp_parser_header.c ./ragel/trtsp_parser_header.rl


# ==Authorization
ragel.exe $OPTIONS -o ./src/headers/trtsp_header_Authorization.c ./ragel/trtsp_parser_header_Authorization.rl

# ==Content-Length
ragel.exe $OPTIONS -o ./src/headers/trtsp_header_Content_Length.c ./ragel/trtsp_parser_header_Content_Length.rl

# ==Content-Type
ragel.exe $OPTIONS -o ./src/headers/trtsp_header_Content_Type.c ./ragel/trtsp_parser_header_Content_Type.rl

# ==CSeq
ragel.exe $OPTIONS -o ./src/headers/trtsp_header_CSeq.c ./ragel/trtsp_parser_header_CSeq.rl

# ==Session
ragel.exe -C -L -T0 -o ./src/headers/trtsp_header_Session.c ./ragel/trtsp_parser_header_Session.rl

# ==Session
ragel.exe -C -L -T0 -o ./src/headers/trtsp_header_Transport.c ./ragel/trtsp_parser_header_Transport.rl

# ==Dummy
ragel.exe $OPTIONS -o ./src/headers/trtsp_header_Dummy.c ./ragel/trtsp_parser_header_Dummy.rl


# ==WWW-Authenticate
ragel.exe $OPTIONS -o ./src/headers/trtsp_header_WWW_Authenticate.c ./ragel/trtsp_parser_header_WWW_Authenticate.rl