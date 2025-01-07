// cld2_emscripten.cc - Embind bindings for exposing functionality to WASM

#include <tuple>
#include <emscripten/bind.h>

#include "lang_script.h"
#include "../public/compact_lang_det.h"

using namespace emscripten;

typedef std::pair<std::string, bool> LanguageDetectionResult;

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

    return std::make_pair(CLD2::LanguageCode(lang), is_reliable);
}

std::vector<LanguageDetectionResult> DetectLanguageBulk(const std::vector<std::string> &texts) {
    std::vector<LanguageDetectionResult> results;

    for (const auto &text : texts) {
        results.push_back(DetectLanguageWrapper(text));
    }

    return results;
}

std::string GetBuildVersion() {
    return CLD2::DetectLanguageVersion();
}

EMSCRIPTEN_BINDINGS(cld2Module) {
    value_object<LanguageDetectionResult>("LanguageDetectionResult")
        .field("langCode", &LanguageDetectionResult::first)
        .field("reliable", &LanguageDetectionResult::second);

    register_vector<std::string>("StringList");
    register_vector<LanguageDetectionResult>("LanguageDetectionResultList");

    function("getVersion", &GetBuildVersion);
    function("detectLanguage", &DetectLanguageWrapper);
    function("detectLanguageBulk", &DetectLanguageBulk);
}
