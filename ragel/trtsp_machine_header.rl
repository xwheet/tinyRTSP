
/**@file trtsp_machine_headers.rl.
 * @brief Ragel file.
 */

/*== Header pasrsing machine. Supports both full-length and compact mode. */
%%{
	machine trtsp_machine_header;
	
	Accept = "Accept"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Accept;
	Accept_Encoding = "Accept-Encoding"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Accept_Encoding;
	Accept_Language = "Accept-Language"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Accept_Language;
	Allow = "Allow"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Allow;
	Authorization = "Authorization"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Authorization;
	Cache_Control = "Cache-Control"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Cache_Control;
	Connection = "Connection"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Connection;
	Content_Base = "Content-Base"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Base;
	Content_Encoding = ("Content-Encoding"i | "e") SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Encoding;
	Content_Language = "Content-Language"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Language;
	Content_Length = "Content-Length"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Length;
	Content_Location = "Content-Location"i  SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Location;
	Content_Type = ("Content-Type"i | "c") SP* HCOLON SP*<: any* :>CRLF  @parse_header_Content_Type;
	CSeq = "CSeq"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_CSeq;
	Date = "Date"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Date;
	Expires = "Expires"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Expires;
	From = "From"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_From;
	Host = "Host"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Host;
	If_Match = "If-Match"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_If_Match;
	If_Modified_Since = "If-Modified-Since"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_If_Modified_Since;
	Last_Modified = "Last-Modified"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Last_Modified;
	Proxy_Authenticate = "Proxy-Authenticate"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Proxy_Authenticate;
	Public = "Public"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Public;
	Range = "Range"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Range;
	Referer = "Referer"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Referer;
	Require = "Require"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Require; 
	Retry_After = "Retry-After"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Retry_After; 
	RTP_Info = "RTP-Info"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_RTP_Info;
	Server = "Server"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Server;
	Session = "Session"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Session; 
	Transport = "Transport"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Transport;
	User_Agent = "User-Agent"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_User_Agent;
	Via = "Via"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_Via;
	WWW_Authenticate = "WWW-Authenticate"i SP* HCOLON SP*<: any* :>CRLF  @parse_header_WWW_Authenticate;
	
	######
	extension_header = (token) SP* HCOLON SP*<: any* :>CRLF  @parse_header_extension_header;
	
	general_header = Cache_Control | Connection | CSeq | Date | Require | Session | Transport | Via;
	auth_header = Authorization | WWW_Authenticate | Proxy_Authenticate;
	request_header = Accept | Accept_Encoding | Accept_Language | From | Host | If_Match | If_Modified_Since | Range | Referer | User_Agent;
	response_header = Public | Retry_After | Server; 
	entity_header = Allow | Content_Base | Content_Encoding | Content_Language | Content_Length | Content_Location | Content_Type | Expires | Last_Modified;

	HEADER = (general_header | auth_header | request_header | entity_header | response_header)@1 | extension_header@0;
}%%