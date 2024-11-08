import type { Metadata } from "next";
import localFont from "next/font/local";
import "./globals.css";

const commodoreFont = localFont({
  src: "./fonts/Bescii-Mono.woff",
});

export const metadata: Metadata = {
  title: "Create Next App",
  description: "Generated by create next app",
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en">
      <body className={`${commodoreFont.className}`}>
        {children}
      </body>
    </html>
  );
}
 