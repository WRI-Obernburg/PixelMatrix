const webpack = require('webpack');
const ESPBuildPlugin = require('./esp/esp-build-plugin');
const purgeCSS = require('@fullhuman/postcss-purgecss');
const tailwindCSS = require('tailwindcss');

export default {
    webpack(config, env, helpers, options) {
        if (env.isProd) {
            config.devtool = false;
            config.plugins = [
                ...config.plugins,
                new ESPBuildPlugin({
                    exclude: [
                        '200.html',
                        'preact_prerender_data.json',
                        'push-manifest.json',
                        "bundle.c4ec2.css.map",
                        "/assets/keeppollyfills.txt",
                    ]
                })
            ];
        };


        const postCssLoaders = helpers.getLoadersByName(config, 'postcss-loader');
        postCssLoaders.forEach(({ loader }) => {
            const plugins = loader.options.postcssOptions.plugins;


            // Add tailwindcss to top of plugins list
            plugins.unshift(tailwindCSS);

            // Purging enabled only during production build
            if (env.production) {
                plugins.push(purgeCSS({
                    content: ['./src/**/*.js', './src/**/*.html', './src/**/*.svg', './src/**/*.jsx'],
                    keyframes: true,
                    defaultExtractor: content => content.match(/[^<>"'`\s]*[^<>"'`\s:]/g) || [],
                }));
            }
        });

        return config;
    }
};
