#ifndef __IMAGE_B9AD55E13BAF45B4904DEE16B97DD41A_H__
#define __IMAGE_B9AD55E13BAF45B4904DEE16B97DD41A_H__

#include <string>
#include <stdint.h>

namespace mediatool
{

class CImage
{
public:
	explicit CImage();
	virtual ~CImage();

public:
	std::string base64Encode(const char * bytes, uint32_t length);
	std::string base64Decode(const std::string &encoded);
	std::string image2base64(const std::string &path);
	bool base64toimage(const std::string &base64, const std::string &dstpath);

private:
	inline bool isBase64(const char c);
};

}

#endif // __IMAGE_B9AD55E13BAF45B4904DEE16B97DD41A_H__
