#include <stddef.h>
#include <ctype.h>

int BrStrcasecmp(char const *lhs, char const *rhs) {
	char lhc = '\0', rhc = '\0';
	do {
		lhc = tolower(*lhs);
		rhc = tolower(*rhs);
		lhs++;
		rhs++;
	} while (lhc == rhc);
	return (unsigned char)lhc - (unsigned char)rhc;
}

int BrStrncasecmp(char const *lhs, char const *rhs, size_t count) {
	char lhc = '\0', rhc = '\0';
	while (count > 0) {
		lhc = tolower(*lhs);
		rhc = tolower(*rhs);
		if (lhc != rhc) {
			break;
		}
		lhs++;
		rhs++;
		count--;
	}
	return (unsigned char)lhc - (unsigned char)rhc;
}
