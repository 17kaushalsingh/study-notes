// @ts-check
import { defineConfig } from 'astro/config';
import { remarkRewriteMarkdownLinks } from './remark-rewrite-links.mjs';

// https://astro.build/config
export default defineConfig({
  markdown: {
    remarkPlugins: [remarkRewriteMarkdownLinks]
  }
});
