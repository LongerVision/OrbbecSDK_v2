// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include "DepthEngineLoader.hpp"
#include "environment/EnvConfig.hpp"
namespace libobsensor {

DepthEngineLoadFactory::DepthEngineLoadFactory() {
    std::string depthEngineLoadPath_ = EnvConfig::getExtensionsDirectory() + "/depthengine/";

    dylib_ = std::make_shared<dylib::library>((depthEngineLoadPath_ + "depthengine").c_str(), dylib::decorations::os_default());

    context_ = std::make_shared<deloader_global_context_t>();

    if(dylib_) {
        try {
            void *symbol = dylib_->get_symbol(K4A_PLUGIN_EXPORTED_FUNCTION);
            
            if(symbol) {
                k4a_register_plugin_fn register_plugin = reinterpret_cast<k4a_register_plugin_fn>(symbol);
                k4a_plugin_t           plugin;
                bool                   result = register_plugin(&plugin);
                if(result) {
                    context_->registerFn = register_plugin;
                    context_->plugin     = plugin;
                    context_->loaded     = true;
                }
            }
        } catch(const dylib::symbol_error& e) {
            context_->loaded = false;
            return;
        } catch(const std::exception& e) {
            context_->loaded = false;
            return;
        }
    }
}

}  // namespace libobsensor
