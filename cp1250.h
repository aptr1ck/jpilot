/*
	Header for Czech library
	Convert Palm  <-> Unix:
		Palm : CP 1250
		Unix : ISO-8859-2
*/

void Win2Lat(unsigned char *const buf, int buf_len);
void Lat2Win(unsigned char *const buf, int buf_len);