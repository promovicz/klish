/*
 * string.c
 */
#include "private.h"

#include <stdlib.h>
#include <string.h>

#include "lub/ctype.h"

const char *lub_string_esc_default = "`|$<>&()#;\\\"!";
const char *lub_string_esc_regex = "^$.*+[](){}";
const char *lub_string_esc_quoted = "\\\"";

/*--------------------------------------------------------- */
void lub_string_free(char *ptr)
{
	if (!ptr)
		return;
	free(ptr);
}

/*--------------------------------------------------------- */
char *lub_string_ndecode(const char *string, unsigned int len)
{
	const char *s = string;
	char *res, *p;
	int esc = 0;

	if (!string)
		return NULL;

	/* Allocate enough memory for result */
	p = res = malloc(len + 1);

	while (*s && (s < (string +len))) {
		if (!esc) {
			if ('\\' == *s)
				esc = 1;
			else
				*p = *s;
		} else {
/*			switch (*s) {
			case 'r':
			case 'n':
				*p = '\n';
				break;
			case 't':
				*p = '\t';
				break;
			default:
				*p = *s;
				break;
			}
*/			*p = *s;
			esc = 0;
		}
		if (!esc)
			p++;
		s++;
	}
	*p = '\0';

	return res;
}

/*--------------------------------------------------------- */
inline char *lub_string_decode(const char *string)
{
	return lub_string_ndecode(string, strlen(string));
}

/*----------------------------------------------------------- */
/*
 * This needs to escape any dangerous characters within the command line
 * to prevent gaining access to the underlying system shell.
 */
char *lub_string_encode(const char *string, const char *escape_chars)
{
	char *result = NULL;
	const char *p;

	if (!escape_chars)
		return lub_string_dup(string);
	if (string && !(*string)) /* Empty string */
		return lub_string_dup(string);

	for (p = string; p && *p; p++) {
		/* find any special characters and prefix them with '\' */
		size_t len = strcspn(p, escape_chars);
		lub_string_catn(&result, p, len);
		p += len;
		if (*p) {
			lub_string_catn(&result, "\\", 1);
			lub_string_catn(&result, p, 1);
		} else {
			break;
		}
	}
	return result;
}

/*--------------------------------------------------------- */
void lub_string_catn(char **string, const char *text, size_t len)
{
	if (text) {
		char *q;
		size_t length, initlen, textlen = strlen(text);

		/* make sure the client cannot give us duff details */
		len = (len < textlen) ? len : textlen;

		/* remember the size of the original string */
		initlen = *string ? strlen(*string) : 0;

		/* account for '\0' */
		length = initlen + len + 1;

		/* allocate the memory for the result */
		q = realloc(*string, length);
		if (NULL != q) {
			*string = q;
			/* move to the end of the initial string */
			q += initlen;

			while (len--) {
				*q++ = *text++;
			}
			*q = '\0';
		}
	}
}

/*--------------------------------------------------------- */
void lub_string_cat(char **string, const char *text)
{
	size_t len = text ? strlen(text) : 0;
	lub_string_catn(string, text, len);
}

/*--------------------------------------------------------- */
char *lub_string_dup(const char *string)
{
	if (!string)
		return NULL;
	return strdup(string);
}

/*--------------------------------------------------------- */
char *lub_string_dupn(const char *string, unsigned int len)
{
	char *res = NULL;

	if (!string)
		return res;
	res = malloc(len + 1);
	strncpy(res, string, len);
	res[len] = '\0';

	return res;
}

/*--------------------------------------------------------- */
int lub_string_nocasecmp(const char *cs, const char *ct)
{
	int result = 0;
	while ((0 == result) && *cs && *ct) {
		/*lint -e155 Ignoring { }'ed sequence within an expression, 0 assumed 
		 * MACRO implementation uses braces to prevent multiple increments
		 * when called.
		 */
		int s = lub_ctype_tolower(*cs++);
		int t = lub_ctype_tolower(*ct++);

		result = s - t;
	}
	/*lint -e774 Boolean within 'if' always evealuates to True 
	 * not the case because of tolower() evaluating to 0 under lint
	 * (see above)
	 */
	if (0 == result) {
		/* account for different string lengths */
		result = *cs - *ct;
	}
	return result;
}

/*--------------------------------------------------------- */
char *lub_string_tolower(const char *str)
{
	char *tmp = strdup(str);
	char *p = tmp;

	while (*p) {
		*p = tolower(*p);
		p++;
	}

	return tmp;
}

/*--------------------------------------------------------- */
const char *lub_string_nocasestr(const char *cs, const char *ct)
{
	const char *p = NULL;
	const char *result = NULL;

	while (*cs) {
		const char *q = cs;

		p = ct;
		/*lint -e155 Ignoring { }'ed sequence within an expression, 0 assumed 
		 * MACRO implementation uses braces to prevent multiple increments
		 * when called.
		 */
		/*lint -e506 Constant value Boolean
		 * not the case because of tolower() evaluating to 0 under lint
		 * (see above)
		 */
		while (*p && *q
		       && (lub_ctype_tolower(*p) == lub_ctype_tolower(*q))) {
			p++, q++;
		}
		if (0 == *p) {
			break;
		}
		cs++;
	}
	if (p && !*p) {
		/* we've found the first match of ct within cs */
		result = cs;
	}
	return result;
}

/*--------------------------------------------------------- */
unsigned int lub_string_equal_part(const char *str1, const char *str2,
	bool_t utf8)
{
	unsigned int cnt = 0;

	if (!str1 || !str2)
		return cnt;
	while (*str1 && *str2) {
		if (*str1 != *str2)
			break;
		cnt++;
		str1++;
		str2++;
	}
	if (!utf8)
		return cnt;

	/* UTF8 features */
	if (cnt && (UTF8_11 == (*(str1 - 1) & UTF8_MASK)))
		cnt--;

	return cnt;
}

/*--------------------------------------------------------- */
const char *lub_string_suffix(const char *string)
{
	const char *p1, *p2;
	p1 = p2 = string;
	while (*p1) {
		if (lub_ctype_isspace(*p1)) {
			p2 = p1;
			p2++;
		}
		p1++;
	}
	return p2;
}

/*--------------------------------------------------------- */
/** @brief Find next word or quoted substring within string
 *
 * @param [in] str String to parse.
 * @param [out] len Length of found substring (without quotes).
 * @param [out] offset Pointer to first symbol after found substring.
 * @param [out] quoted Is substring quoted?
 * @param [out] qclosed Is closed quotes found?
 * @return Pointer to found substring (without quotes).
 */
const char *lub_string_nextword(const char *str,
	size_t *len, const char **offset, bool_t *quoted, bool_t *qclosed,
	bool_t *altq)
{
	const char *string = str;
	const char *word = NULL;
	const char dbl_quote = '"';
	bool_t dbl_quoted = BOOL_FALSE;
	const char alt_quote = '`';
	unsigned int alt_quote_num = 0; // Number of opening alt quotes
	bool_t alt_quoted = BOOL_FALSE;
	bool_t closed_quote = BOOL_FALSE;
	size_t length = 0;

	// Find the start of a word (not including an opening quote)
	while (*string && isspace(*string))
		string++;

	// Is this the start of a quoted string?
	if (*string == dbl_quote) {
		dbl_quoted = BOOL_TRUE;
		string++;
	} else if (*string == alt_quote) {
		alt_quoted = BOOL_TRUE;
		while (string && (*string == alt_quote)) {
			string++;
			alt_quote_num++; // Count starting quotes
		}
	}
	word = string;

	// Find the end of the word
	while (*string) {

		// Standard double quotation
		if (dbl_quoted) {
			// End of word
			if (*string == dbl_quote) {
				closed_quote = BOOL_TRUE;
				string++;
				break;
			}

		// Alternative multi quotation
		} else if (alt_quoted) {
			unsigned int qnum = alt_quote_num;
			while (string && (*string == alt_quote) && qnum) {
				string++;
				length++;
				qnum--;
			}
			if (0 == qnum) { // End of word was found
				// Quotes themselfs are not a part of a word
				length -= alt_quote_num;
				closed_quote = BOOL_TRUE;
				break;
			}
			if (qnum != alt_quote_num) // Skipped some qoute symbols
				continue;

		// Not quoted
		} else {
			// End of word
			if (isspace(*string))
				break;
		}

		// Common case
		// Escaping. It doesn't work within alt quoting
		if (!alt_quoted && (*string == '\\')) {
			// Skip escaping
			string++;
			length++;
			// Skip escaped symbol
			if (*string) {
				length++;
				string++;
			}
			continue;
		}

		length++;
		string++;
	}

	// Skip strange symbols after quotation
	while (*string && !isspace(*string))
		string++;

	if (len)
		*len = length;
	if (offset)
		*offset = string;
	if (quoted)
		*quoted = dbl_quoted || alt_quoted;
	if (qclosed)
		*qclosed = closed_quote;
	if (altq)
		*altq = alt_quoted;

	return word;
}

/*--------------------------------------------------------- */
unsigned int lub_string_wordcount(const char *line)
{
	const char *word = NULL;
	unsigned int result = 0;
	const char *offset = NULL;

	for (word = lub_string_nextword(line, NULL, &offset, NULL, NULL, NULL);
		word && (*word != '\0');
		word = lub_string_nextword(offset, NULL, &offset, NULL, NULL, NULL)) {
		result++;
	}

	return result;
}

/*--------------------------------------------------------- */
