# Chapter 9: Application Development

## 1. Web Fundamentals

Most database access today occurs via web or mobile applications.

- **Uniform Resource Locator (URL)**: A globally unique name for a document or resource on the web.
- **HTTP (HyperText Transfer Protocol)**: The communication protocol between browsers and web servers.
  - **GET**: Encodes parameters in the URL.
  - **POST**: Encodes parameters in the body of the request (more secure for data entry).
- **HTML/CSS**: Languages used to structure and style the user interface.

---

## 2. Server-Side Technologies

Web servers execute programs to process requests and interact with databases.

- **Servlets**: Java classes that run on a web server to process HTTP requests.
- **JSP (Java Server Pages)**: Allows embedding Java code within HTML documents.
- **Django (Python)**: A high-level web framework that encourages rapid development and clean design.
- **Business Logic Layer**: The part of the application that enforces rules (e.g., ensuring a student has enough credits to enroll).

---

## 3. Client-Side Technologies

Code that runs directly in the user's browser.

- **JavaScript**: The primary client-side scripting language.
- **Ajax (Asynchronous JavaScript and XML)**: Allows web pages to be updated asynchronously by exchanging data with a server in the background without reloading the page.
- **Mobile Platforms**: Native apps (Android/iOS) or cross-platform frameworks (React Native, Flutter).

---

## 4. Application Architecture

Modern applications usually follow a **Three-Layer Architecture**:
1. **Presentation Layer**: Handles user interaction (Browser/UI).
2. **Business-Logic Layer**: Implements the core functionality and rules.
3. **Data-Access Layer**: Handles communication with the database (using JDBC, ORMs, etc.).

---

## 5. Performance and Security

### Performance Optimization
- **Caching**: Storing frequently accessed data in memory (e.g., **Memcached**, **Redis**) to avoid expensive database queries.
- **Connection Pooling**: Maintaining a "pool" of open database connections to be reused by different user sessions, reducing the overhead of opening new connections.

### Security Best Practices
- **Authentication**: Verifying the user's identity (e.g., two-factor authentication, single sign-on).
- **SQL Injection**: Prevented by using Prepared Statements.
- **Cross-Site Scripting (XSS)**: An attack where malicious scripts are injected into web pages. Prevented by escaping user input.
- **Cross-Site Request Forgery (CSRF)**: Forcing an authenticated user to execute unwanted actions. Prevented by using unique tokens for every session.

---

## Must-Know Things
- **MVC Pattern**: Model-View-Controller is the standard architectural pattern for web development.
- **Statelessness of HTTP**: Browsers use **Cookies** and **Sessions** to maintain state across different requests.
- **Encryption**: Sensitive data (like passwords) should never be stored in plain text; they should be **hashed** using strong algorithms (e.g., bcrypt).

---

## Quick Revision
- **Front-end**: HTML, CSS, JavaScript, Ajax.
- **Back-end**: Servlets, Django, Node.js.
- **Architecture**: 3-Layer (UI $ightarrow$ Logic $ightarrow$ Data).
- **Security**: Sanitize inputs, use HTTPS, hash passwords, use CSRF tokens.
- **Performance**: Use Caching and Connection Pooling.
