"""Convert .svg images to .pdf so the LaTeX builder can embed them.

The docs draw a lot of FSTs as .svg. The LaTeX builder only supports
pdf/png/jpeg, so without this the figures would silently vanish from the
generated PDF.

It uses ``rsvg-convert`` from librsvg:

    - Ubuntu: sudo apt-get install librsvg2-bin
    - macOS:  brew install librsvg
    - Alpine: apk add librsvg

refer to
https://www.sphinx-doc.org/en/master/extdev/appapi.html#sphinx.application.Sphinx.add_post_transform
"""

import subprocess
from typing import Any, Dict

from sphinx.application import Sphinx
from sphinx.errors import ExtensionError
from sphinx.locale import __
from sphinx.transforms.post_transforms.images import ImageConverter
from sphinx.util import logging

logger = logging.getLogger(__name__)


class RsvgConverter(ImageConverter):
    conversion_rules = [
        ("image/svg+xml", "application/pdf"),
    ]

    def is_available(self) -> bool:
        try:
            args = ["rsvg-convert", "--version"]
            logger.debug("Invoking %r ...", args)
            subprocess.run(
                args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True
            )
            return True
        except (OSError, subprocess.CalledProcessError) as exc:
            logger.warning(
                __(
                    "Unable to run %r. Please install librsvg so that "
                    "rsvg-convert is in your PATH.\n\nTraceback: %s"
                ),
                "rsvg-convert",
                exc,
            )
            return False

    def convert(self, _from: str, _to: str) -> bool:
        try:
            args = ["rsvg-convert", "-f", "pdf", "-o", _to, _from]
            logger.debug("Invoking %r ...", args)
            subprocess.run(
                args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True
            )
            return True
        except OSError:
            logger.warning(
                __("rsvg-convert cannot be run; is librsvg installed?"),
            )
            return False
        except subprocess.CalledProcessError as exc:
            raise ExtensionError(
                __(
                    "rsvg-convert exited with error:\n[stderr]\n%r\n[stdout]\n%r"
                )
                % (exc.stderr, exc.stdout)
            ) from exc


def setup(app: Sphinx) -> Dict[str, Any]:
    app.add_post_transform(RsvgConverter)

    return {
        "version": "0.1",
        "parallel_read_safe": True,
        "parallel_write_safe": True,
    }
