# cld2-wasm
Building the cld2 library targeting WebAssembly for use in browsers.

## Setup
1. Download cld2.js and cld2.wasm to your project, placing cld2.wasm so it can be loaded from `http://<your-server-location>/cld2.wasm`
2. Import cld2, `import cld2Module from './cld2.js'`
3. Load the WASM module using `const cld2 = await cld2Module()`

##### `cld2.getVersion()`
Returns the current source build version (should be `V2.0 - 20141016`)

##### `cld2.detectLanguage(text)`
Attempts to detect the most probable language for the given text, returning the code and whether it's reliable.

```js
const {langId, langCode, isReliable} = cld2.detectLanguage("The quick brown fox jumps over the lazy dog");
console.log(`Detected language ${langCode} (${isReliable ? "reliable result" : "not confident"})`);
```

##### `cld2.UNKNOWN_LANGUAGE_ID`
(Constant) The unknown language id

##### `cld2.getLanguageName(langId)`
Given a langauge id (integer), returns the formatted name

```js
const unknownLangName = cld2.getLanguageName(cld2.UNKNOWN_LANGUAGE_ID);

// unknownLangName = "Unknown"
```

## Building
1. Shallow clone the original repo `git clone --depth=1 https://github.com/CLD2Owners/cld2.git`
2. `cd` into cld2
3. Add `cld2_emscripten.cc` to internal folder
4. Using emcc, build to WASM
```bash
docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) -w /src/internal emscripten/emsdk \
  emcc -O3 -w -Wno-narrowing -lembind -o ../cld2.js \
  -s EXPORT_ES6=1 -s MODULARIZE=1 -s EXPORT_NAME="'cld2'" \
  cldutil.cc cldutil_shared.cc compact_lang_det.cc compact_lang_det_hint_code.cc \
  compact_lang_det_impl.cc debug.cc fixunicodevalue.cc \
  generated_entities.cc generated_language.cc generated_ulscript.cc  \
  getonescriptspan.cc lang_script.cc offsetmap.cc  scoreonescriptspan.cc \
  tote.cc utf8statetable.cc  \
  cld_generated_cjk_uni_prop_80.cc cld2_generated_cjk_compatible.cc  \
  cld_generated_cjk_delta_bi_4.cc generated_distinct_bi_0.cc  \
  cld2_generated_quadchrome_2.cc cld2_generated_deltaoctachrome.cc \
  cld2_generated_distinctoctachrome.cc  cld_generated_score_quad_octa_2.cc cld2_emscripten.cc
```
4. collect cld2.js and cld2.wasm from the cld2 folder

## License
Licensed under MIT. See license.md for more info.
