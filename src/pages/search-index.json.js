import fs from 'node:fs';

export async function GET() {
  const postsMap = import.meta.glob('../../notes/systes-and-backend/backend-from-first-principles/*.md', { eager: true });
  const posts = Object.values(postsMap);
  
  const index = posts
    .filter(post => {
      const filename = post.file.split('/').pop() || '';
      return filename.toLowerCase().startsWith('lec') && filename.endsWith('.md');
    })
    .map(post => {
      const filename = post.file.split('/').pop() || '';
      const slug = filename.replace(/\.md$/, '').toLowerCase();
      const headings = post.getHeadings();
      const h1 = headings.find(h => h.depth === 1);
      let title = h1 ? h1.text : filename.replace(/\.md$/, '');
      title = title.replace(/\s*[—-]\s*Lecture Notes/i, '');
      const lecNum = filename.match(/LEC(\d+(?:\.\d+)?)/i)?.[1] || '';
      
      // Read raw file content to index it for full-text search!
      let contentExcerpt = '';
      try {
        const rawContent = fs.readFileSync(post.file, 'utf-8');
        // Clean markdown syntax for indexing
        contentExcerpt = rawContent
          .replace(/[#*`_\[\]()\-]/g, ' ')
          .replace(/\s+/g, ' ')
          .slice(0, 4000); // Grab first 4000 chars to keep search payload optimized
      } catch (err) {
        contentExcerpt = title;
      }
      
      return {
        title: `Lec ${lecNum}: ${title}`,
        slug,
        lecNum,
        excerpt: contentExcerpt,
        url: `/systems-and-backend/backend-from-first-principles/${slug}`
      };
    });

  return new Response(JSON.stringify(index), {
    headers: { 'Content-Type': 'application/json' }
  });
}
