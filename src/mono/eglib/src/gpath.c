/*
 * Portable Utility Functions
 *
 * Author:
 *   Miguel de Icaza (miguel@novell.com)
 *
 * (C) 2006 Novell, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <glib.h>

gchar *
g_build_path (const gchar *separator, const gchar *first_element, ...)
{
	GString *result;
	const char *s, *p, *next;
	int slen;
	va_list args;
	
	g_return_val_if_fail (separator != NULL, NULL);
	g_return_val_if_fail (first_element != NULL, NULL);

	result = g_string_sized_new (48);

	slen = strlen (separator);
	
	va_start (args, first_element);
	for (s = first_element; s != NULL; s = next){
		next = va_arg (args, char *);
		p = (s + strlen (s));

		if (next && p - slen > s){
			for (; strncmp (p-slen, separator, slen) == 0; ){
				p -= slen;
			}
		}
		g_string_append_len (result, s, p - s);

		if (next){
			g_string_append (result, separator);

			for (; strncmp (next, separator, slen) == 0; )
				next += slen;
		}
	}
	va_end (args);

	return g_string_free (result, FALSE);
}

gchar *
g_path_get_dirname (const gchar *filename)
{
	char *p, *r;
	int count;
	g_return_val_if_fail (filename != NULL, NULL);

	p = strrchr (filename, G_DIR_SEPARATOR);
	if (p == NULL)
		return g_strdup ("");
	count = p - filename;
	r = g_malloc (count + 1);
	strncpy (r, filename, count);
	r [count] = 0;

	return r;
}

gchar *
g_path_get_basename (const char *filename)
{
	char *r;
	g_return_val_if_fail (filename != NULL, NULL);

	/* Empty filename -> . */
	if (!*filename)
		return g_strdup (".");

	/* No separator -> filename */
	r = strrchr (filename, G_DIR_SEPARATOR);
	if (r == NULL)
		return g_strdup (filename);

	/* Trailing slash, remove component */
	if (r [1] == 0){
		char *copy = g_strdup (filename);
		copy [r-filename] = 0;
		r = strrchr (copy, G_DIR_SEPARATOR);

		if (r == NULL){
			g_free (copy);			
			return g_strdup ("/");
		}
		r = g_strdup (&r[1]);
		g_free (copy);
		return r;
	}

	return g_strdup (&r[1]);
}
