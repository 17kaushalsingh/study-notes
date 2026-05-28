import { visit } from 'unist-util-visit';
import path from 'path';

export function remarkRewriteMarkdownLinks() {
  return (tree, file) => {
    visit(tree, 'link', (node) => {
      const url = node.url;
      // Match relative links like ./LEC5.md, ./BEST_PRACTICES.md, etc.
      if (url && (url.startsWith('./') || url.startsWith('../') || (!url.startsWith('http') && url.endsWith('.md')))) {
        // Only rewrite if it links to a markdown file
        if (url.endsWith('.md') || url.includes('.md#')) {
          const hashIndex = url.indexOf('#');
          const anchor = hashIndex !== -1 ? url.substring(hashIndex) : '';
          const cleanUrl = hashIndex !== -1 ? url.substring(0, hashIndex) : url;

          const filename = path.basename(cleanUrl);
          const slug = filename.replace(/\.md$/, '').toLowerCase();
          
          // Generate a clean hosted route
          const newUrl = `/systems-and-backend/backend-from-first-principles/${slug}${anchor}`;
          node.url = newUrl;
        }
      }
    });
  };
}
