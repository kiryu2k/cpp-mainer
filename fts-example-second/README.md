All the elements below are reworked parts of the code designed to create an individual project.

Given that there is one task for everyone, it was not possible to create the newest project.

The structure for storing the results of parsing has been redesigned - now it's just 2 fields: for ngram and its position in the text (there used to be an ngram vector).

The representation of the inverse index in memory has been changed. Now it is an unordered map with a pair of terms and a vector of the TermInfo structure, which stores the document ID and the vector of the term positions in it.
Because of this, I had to change the functions associated with the reverse index, adapting them to this representation.

Tests were rewritten and the structure of reading text indexes was removed because it became unnecessary.