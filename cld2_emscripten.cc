// cld2_emscripten.cc - Embind bindings for exposing functionality to WASM

#include <tuple>
#include <emscripten/bind.h>

#include "lang_script.h"
#include "../public/compact_lang_det.h"

using namespace emscripten;

typedef struct {
    int lang;
    std::string langCode;
    bool isReliable;
} LanguageDetectionResult;

LanguageDetectionResult DetectLanguageWrapper(const std::string &text) {
    CLD2::Language lang;
    int percent3[3];
    double normalized_score3[3];
    CLD2::ResultChunkVector resultchunkvector;
    int text_bytes;
    bool is_reliable;
    int valid_prefix_bytes;

    lang = CLD2::ExtDetectLanguageSummaryCheckUTF8(
        text.c_str(),
        text.size(),
        true,
        nullptr,
        0,
        nullptr,
        percent3,
        normalized_score3,
        &resultchunkvector,
        &text_bytes,
        &is_reliable,
        &valid_prefix_bytes
    );

    return {lang, CLD2::LanguageCode(lang), is_reliable};
}

std::string GetBuildVersion() {
    return CLD2::DetectLanguageVersion();
}

std::string GetLanguageName(int lang) {
    return CLD2::LanguageName(static_cast<CLD2::Language>(lang));
}

EMSCRIPTEN_BINDINGS(cld2Module) {
  constant("UNKNOWN_LANGUAGE_ID", static_cast<int>(CLD2::UNKNOWN_LANGUAGE));

  value_object<LanguageDetectionResult>("LanguageDetectionResult")
      .field("langId", &LanguageDetectionResult::lang)
      .field("langCode", &LanguageDetectionResult::langCode)
      .field("isReliable", &LanguageDetectionResult::isReliable);

  function("getVersion", &GetBuildVersion);
  function("getLanguageName", &GetLanguageName);

  function("detectLanguage", &DetectLanguageWrapper);
}

