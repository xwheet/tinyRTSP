/**@file trtsp_machine_message.rl
 * @brief Ragel file.
 */
%%{
	machine trtsp_machine_message;

	message_body = any*;
	RTSP_Version = ("RTSP"i "/" DIGIT+ "." DIGIT+)>tag %parse_rtspversion;
	
	message_header = any+ >tag :>CRLF %parse_header;
	
	# RTSP RESPONSE
	Reason_Phrase = (( reserved | unreserved | escaped | UTF8_NONASCII | UTF8_CONT | SP | HTAB )*)>tag %parse_reason_phrase;
	Status_Line = RTSP_Version :>SP Status_Code>tag %parse_status_code :>SP Reason_Phrase :>CRLF;
	Response = Status_Line (message_header* :>CRLF);
	
	# RTSP REQUEST
	URI = (any+)>tag %parse_requesturl;
	Request_URI = URI;
	Request_Line = Method>tag %parse_method :>SP Request_URI :>SP RTSP_Version :>CRLF;
	Request = Request_Line (message_header* :>CRLF);

	# RTSP MESSAGE
	RTSP_message = (Response | Request)>1 @eoh message_body?>0;
}%%