# BM25 Text Search

A compact C++20 full-text search library and CLI implementing BM25 relevance ranking. It tokenizes documents, tracks term and document frequencies, normalizes document length, and returns deterministic ranked results.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

## Search a text collection

Place UTF-8 `.txt` files in a directory and run:

```bash
./build/bm25-search ./documents "cell membrane transport"
```

Output contains the BM25 score and relative document path, highest relevance first.

## Library use

```cpp
unays::BM25Index index;
index.add_document("lesson-1", "Cells are the basic units of living things.");
auto results = index.search("living cells", 5);
```

This starter tokenizer handles ASCII letters and digits. Production multilingual search should add Unicode-aware normalization, stemming, language-specific tokenization, persistence, and incremental indexing.

## License

MIT License.
