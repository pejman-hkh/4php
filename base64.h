#include <string.h>

static const char base64_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
};

static const char base64_pad = '=';

static const short base64_reverse_table[256] = {
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
	-2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
	-2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
};

std::string php_base64_encode(std::string &current, size_t length)
{

	std::string p;


	while (length > 2) { 
		p += base64_table[current[0] >> 2];
		p += base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		p += base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		p += base64_table[current[2] & 0x3f];

		current += 3;
		length -= 3; 
	}


	if (length != 0) {
		p += base64_table[current[0] >> 2];
		if (length > 1) {
			p += base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			p += base64_table[(current[1] & 0x0f) << 2];
			p += base64_pad;
		} else {
			p += base64_table[(current[0] & 0x03) << 4];
			p += base64_pad;
			p += base64_pad;
		}
	}

	return p;
}


std::string php_base64_decode( std::string &current, size_t length, bool strict = 0 )
{

	int ch, i = 0, j = 0, padding = 0;
	std::string result;
	std::string out;
	
	while (length-- > 0) {
		ch = current[i];

		if (ch == base64_pad) {
			padding++;
			continue;
		}

		ch = base64_reverse_table[ch];
		if (!strict) {
			
			if (ch < 0) {
				continue;
			}
		} else {
		
			if (ch == -1) {
				continue;
			}
		
			if (ch == -2 || padding) {
				goto fail;
			}
		}

		switch(i % 4) {
		case 0:
			result[j] = ch << 2;
			break;
		case 1:
			result[j++] |= ch >> 4;
			result[j] = (ch & 0x0f) << 4;
			break;
		case 2:
			result[j++] |= ch >>2;
			result[j] = (ch & 0x03) << 6;
			break;
		case 3:
			result[j++] |= ch;
			break;
		}
		i++;
	}

	
	for( int s = 0; s < j; s++ ) {
		out += result[s];
	}
	

	if (strict && i % 4 == 1) {
		goto fail;
	}
	
	if (strict && padding && (padding > 2 || (i + padding) % 4 != 0)) {
		goto fail;
	}

	return out;

fail:
	std::cout << "fail" << std::endl;
	return NULL;
}
